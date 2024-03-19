//------------------------------------------------------------------------------------------------------
#include "FSampProfiler.h"
#include "XML\\FXMLStream.h"
//------------------------------------------------------------------------------------------------------
int FSampProfiler::FSample_compare(const void *arg1, const void *arg2)
{
	FSample* s1 = *(FSample**)arg1;
	FSample* s2 = *(FSample**)arg2;
	assert(s1->CallStackDepth && s2->CallStackDepth);
	if(s1->CallStack[0] > s2->CallStack[0]) return 1;
	if(s1->CallStack[0] < s2->CallStack[0]) return -1;
	return 0;
}
//------------------------------------------------------------------------------------------------------
void FSampProfiler::SaveToFile(char* FileName)
{
	assert(RootFunctionIdx != -1);
	void* Base = GetCurrentModuleBase();

	f_xmlQuote* root_xml = new f_xmlQuote();
	root_xml->AssignName("Root");

	FSample** Samples = new FSample*[SamplesNumber];
	int snum = 0;
	FSample* pS = FirstSample;
	while(pS)
	{
		Samples[snum++] = pS;
		pS = pS->Next;
	}
	qsort(Samples, SamplesNumber, sizeof(FSample*), FSample_compare);

	// - идем по всем семплам
	for(int s = 0; s < SamplesNumber; s++)
	{
		FSample* Sample = Samples[s];
		// - начинаем с корня
		f_xmlQuote* xml = root_xml;
		// - идем по цепочке вызовов текущего семпла
		int Depth = Sample->CallStackDepth;
		for(int k = 0; k < Depth; k++)
		{
			// - получим адрес внутри вызванной функции
			// (очередной адрес возврата, либо базовый адрес функции для последнего элемента)
			unsigned TargetAddr = Sample->CallStack[k];
			unsigned TargetFunctionBase = NULL;
			char* TargetFuncName = "-Undefined-";
			// - получим индекс вызванной функции
			int TargetFunctionIdx = FindFunctionByVA(TargetAddr);
			if(TargetFunctionIdx != -1)
			{
				// - получаем базовый адрес вызванной функции
				TargetFunctionBase = Functions[TargetFunctionIdx]->Base;
				// - определим имя вызванной функции 
				TargetFuncName = GetFunctionByRVA(TargetAddr - *(unsigned*)&Base);
			}
			else
			{
				// - ищем среди модулей
				unsigned* Data = (unsigned*)ModulesStream.Data;
				int Number = ModulesStream.Size/(4*3);

				for(int i = 0; i < Number; i++)
				{
					unsigned Base = Data[i*3+0];
					unsigned Size = Data[i*3+1];
					char* Str = (char*)(Data[i*3+2]);
					// - ищем модуль, в котором было прерывание
					if(TargetAddr >= Base && TargetAddr < Base + Size)
					{
						// - нашли модуль
						char* p = strrchr(Str, '\\');
						if(p) Str = p+1;
						TargetFuncName = Str;
						TargetFunctionBase = Base;
						f_xmlQuote* _xml = xml->GetSubQuote("Modules");
						if(_xml == NULL) _xml = xml->AddSubQuote("Modules");
						xml = _xml;
						break;
					}
				}
			}

			// - получим адрес возврата из текущей функции
			unsigned ReturnAddr = 0;
			if(k > 0) ReturnAddr = Sample->CallStack[k-1];
			// - пробуем найти такую запись
			f_xmlQuote* _xml = NULL;
			int N = xml->GetNSubQuotes();
			for(int n = 0; n < N; n++)
			{
				// - идем по всем дочерним элементам первого уровня
				f_xmlQuote* __xml = xml->GetSubQuote(n);
				assert(__xml);
				// - проверка на тег-функцию
				char* QuoteName = (char*)__xml->GetQuoteName();
				assert(QuoteName);
				if(strcmp(QuoteName, "Function") == 0)
				{
					// - проверка имени функции
					char* NameValue = __xml->GetAttributeValue("Name");
					assert(NameValue);
					if(strcmp(NameValue, TargetFuncName) == 0)
					{
						// - проверка адреса возврата
						char* ReturnAddressValue = __xml->GetAttributeValue("RetAddr");
						assert(ReturnAddressValue);
						unsigned ReturnAddress = 0;
						sscanf(ReturnAddressValue, "%x", &ReturnAddress);
						if(ReturnAddress == ReturnAddr)
						{
							// - проверка базового адреса
							char* BaseAddressValue = __xml->GetAttributeValue("BaseAddr");
							assert(BaseAddressValue);
							unsigned BaseAddress = 0;
							sscanf(BaseAddressValue, "%x", &BaseAddress);
							if(BaseAddress == TargetFunctionBase)
							{
								_xml = __xml;
								break;
							}
						}
					}
				}
			}
			// - если нет, то создаем запись
			if(_xml == NULL)
			{
				_xml = xml->AddSubQuote("Function");
				_xml->AddAttribute("Name", TargetFuncName);
			}
			xml = _xml;
			// - берем, если есть, количество семплов
			int OldSamples = 0;
			char* SamplesValue = xml->GetAttributeValue("Samples");
			if(SamplesValue) OldSamples = atoi(SamplesValue);
			xml->isHex = false;
			xml->AddAttribute("Samples", OldSamples + Sample->Count);

			if(k == Depth-1)
			{
				assert(xml->GetAttributeValue("OwnSamples") == NULL);
				xml->AddAttribute("OwnSamples", Sample->Count);
			}
			xml->isHex = true;
			xml->AddAttribute("RetAddr", ReturnAddr);
			xml->AddAttribute("BaseAddr", TargetFunctionBase);
		}
	}
	f_xmlQuote* _xml = NULL;
	f_xmlQuote* xml = root_xml->AddSubQuote("Function");
	xml->isHex = false;
	xml->AddAttribute("Name", "!Times");
	xml->AddAttribute("Samples", SamplingsCount);
	char buf[128];
	for(int i = 0; i < TimeDistributionInterval; i++)
	{
		sprintf(buf, "!timeDelta_%03u-%03u", i, i+1);
		_xml = xml->AddSubQuote("Function");
		_xml->isHex = false;
		_xml->AddAttribute("Name", buf);
		_xml->AddAttribute("Samples", TimeDistribution[i]);
	}
	sprintf(buf, "!timeDelta_%03u-INF", TimeDistributionInterval);
	_xml = xml->AddSubQuote("Function");
	_xml->AddAttribute("Name", buf);
	_xml->AddAttribute("OwnSamples", TimeDistribution[TimeDistributionInterval]);

	// - store the information
	{
		f_xmlQuote* xml = root_xml->AddSubQuote("Info");
		char buf[128];
		if(SamplingsCount)
		{
			// - время выполнения всех семплингов в миллисекундах
			double samplingTime = SamplingsTime;

			// - время всей работы в миллисекундах
			double workTime = fpWork.GetDeltaTime();

			// - запишем время работы в секундах
			sprintf(buf, "%.3f", (float)(workTime/1000.0));
			xml->AddAttribute("sWorkTime", buf);

			// - запишем количество семплингов
			xml->isHex = false;
			xml->AddAttribute("AllSamplingsCount", SamplingsCount);

			// - запишем количество миллисекунд в одном семплинге
			sprintf(buf, "%.3f", (float)(samplingTime/(double)SamplingsCount));
			xml->AddAttribute("msOneSamplingTime", buf);

			// - запишем среднее количество миллисекунд между семплингами
			sprintf(buf, "%.3f", (float)(AverTimeBetweenSamplings/(double)SamplingsCount));
			xml->AddAttribute("msAverTimeBetweenSamplings", buf);

			// - запишем минимальное количество миллисекунд между семплингами
			sprintf(buf, "%.3f", (float)MinTimeBetweenSamplings);
			xml->AddAttribute("msMinTimeBetweenSamplings", buf);

			// - запишем максимальное количество миллисекунд между семплингами
			sprintf(buf, "%.3f", (float)MaxTimeBetweenSamplings);
			xml->AddAttribute("msMaxTimeBetweenSamplings", buf);

			// - запишем процент использования общего времени
			sprintf(buf, "%.3f%%", (float)(100.0*samplingTime/workTime));
			xml->AddAttribute("CpuUsage", buf);

			xml->isHex = false;
			xml->AddAttribute("MissedSamples", MissedSamples);
		}
	}
	root_xml->WriteToFile(FileName);
	delete root_xml;
	delete[] Samples;

	#ifdef FSAMP_PROFILER_LOG
		f_FInStream TimesInStream;
		TimesStream.remount(&TimesInStream);
		FILE* out = fopen("!Times!.txt", "wt");
		assert(out);
		double startTime = 0;
		for(unsigned i = 0; i < SamplingsCount; i++)
		{
			double currTime = (double)TimesInStream.getint()/1000.0;
			double deltaTime = (double)TimesInStream.getint()/1000.0;
			if(i == 0) startTime = currTime;
			currTime -= startTime;
			fprintf(out, "%f %f\n", (float)currTime, (float)i/(float)SamplingsCount);//deltaTime);
		}
	#endif
}
//------------------------------------------------------------------------------------------------------

//
////-------------------------------------------------------------------------------------------------------------------
//// интерфейс создания нового, выбора или удаления существующего игрока
////-------------------------------------------------------------------------------------------------------------------
//
//bool	OnPlayerNameClicked(SimpleDialog* pSD); 
//bool	OnPIButtonClicked(SimpleDialog* pSD);
//bool	PlayerIntrfAddNewPlayer(DialogsSystem* pMenu,int* IntrfActiveNation,int* IntrfActiveDiff,char** name,int* heroId);
//bool	PlayerIntrfDeletePlayer(DialogsSystem* pMenu, char* name, int nation);
//
////bool	OnNewPlayerClicked(SimpleDialog* pSD);
////bool	OnDeletePlayerClicked(SimpleDialog* pSD);
////bool	OnNextHeroClicked(SimpleDialog* pSD);
//
// 
////// основное меню игрока
//bool PlayerInterface()
//{
//	KeyPressed = 0;		//Global
//    LastKey    = 0;		//Global
//
//	int X0 = 420;					//координатa X0
//	int Y0 = 260;					//координатa Y0
//	const int MAXNAMES = 50;		//max кол-во имен игроков
//
//	int Active = -1;				//выбраное имя игрока
//	int ActiveNation  = 0;			//текущая нация в меню
//	int ActiveDiff    = 1;			//текущий ур.сложн. в меню
//
//	CPlayerSAVE	SAVE; 
//
//	//// loading from XML			/* LOAD SAVED "bmplsav.xml" (BigMap Players Save) */
//	//xmlQuote xml;
//	//xml.ReadFromFile("Players\\bmplsav.xml");
//	//ErrorPager Error;
//	//SAVE.Load(xml,&SAVE,&Error);
//
//	int Names = SAVE.m_pPlayer.GetAmountOfElements(); 
//
//	GP_TextButton*  a_ptbName[MAXNAMES];	// Names
//	GPPicture*		a_pgppFlag[MAXNAMES];	// National Flags
//	GPPicture*		a_pgppColor[MAXNAMES];	// in-game colors
//	
//	for(int i=0; i<MAXNAMES; i++) { a_ptbName[i]=0x00000000; a_pgppFlag[i]=0x00000000; a_pgppColor[i]=0x00000000; }
//
//
//	DialogsSystem   a_dsMenu(0,0);							// main menu
//
//	//фон
//	SQPicture   a_sqpBack( "Interf3\\TotalWarGraph\\Player_Interface.bmp" ); //фон
//	a_dsMenu.addPicture(NULL,0,0,&a_sqpBack,&a_sqpBack,&a_sqpBack);
//	
//	//надпись " Игроки/Players "
//	TextButton* a_ptbHead = a_dsMenu.addTextButton(NULL,X0+245,Y0+(-66),GetTextByID("#CIT_PlayerNames"),&YellowFont,&YellowFont,&YellowFont,1); //NEED NEW FONTS		
//
//	//вывод имен, флагов и цветов игроков
//	DialogsDesk*	a_pddDesk = a_dsMenu.AddDialogsDesk(X0, Y0, 460, 250, "EmptyBorder");
//
//	LocalGP		    a_lgpNameButton("Interf3\\TotalWarGraph\\int_PlayerName");	//фон для имен игроков
//	LocalGP			a_lgpFlags("Interf3\\TotalWarGraph\\lva_Flags");			//флаги
//	LocalGP			a_lColorValue("Interf2\\PlayerList");							//player color
//	
//	int dX0 = 10; int dY0 = 5; //10;	
//	for(int i=0; i<Names; i++) 
//	{
//		//вывод имен игроков	UniversalButton* TBS[i]=MENU.addUniversalButton(XL0,YL0,467,ccc,TB1.GPID,Set[i%3],0,0,0,FYellow,FYellow,FWhite);
//		a_ptbName[i] = a_pddDesk->addGP_TextButton(NULL,dX0,dY0+35*i/*27*i*/,SAVE.m_pPlayer[i]->m_chName.pchar(),a_lgpNameButton.GPID,4/*0*/,&WhiteFont,&YellowFont /*&BlackFont*/);//NEED NEW FONTS
//		a_ptbName[i]->Sprite  = 6;	//1; //active
//		a_ptbName[i]->Sprite1 = 4;	//0; //passive
//		a_ptbName[i]->Center  = false;
//		
//		//int width = 200; int height = 27;
//		//a_ptbName[i]->x  = dX0; //a_ptbName[i]->y  = dY0 + height*i; //a_ptbName[i]->x1 = dX0 + width; //a_ptbName[i]->y1 = dY0 + height*(i+1); 
//		
//		a_ptbName[i]->FontDy    = -3; //5; 
//		a_ptbName[i]->UserParam = 0; 
//		a_ptbName[i]->OnClick   = &OnPlayerNameClicked;
//		
//		//вывод флагов игроков
//		a_pgppFlag[i] = a_pddDesk->addGPPicture(NULL, a_ptbName[i]->x+300,a_ptbName[i]->y-5 ,a_lgpFlags.GPID, SAVE.m_pPlayer[i]->m_iNation+1 );
//
//		//colors
//		a_pgppColor[i] = a_pddDesk->addGPPicture(NULL, a_ptbName[i]->x+350,a_ptbName[i]->y+3 ,a_lColorValue.GPID, SAVE.m_pPlayer[i]->m_iNation );
//	}
//
//	if(Names>0) OnPlayerNameClicked(a_ptbName[0]);
//
//	//кнопки new/del/ok/cancel
//	LocalGP		a_lgpButtons("Interf3\\TotalWarGraph\\dMessage");
//
//	int bX0 = X0 + 10;  //X0 += 50; 
//	int bY0 = Y0 + 260; //240;
//	a_dsMenu.addGPPicture(NULL, bX0,bY0 ,a_lgpButtons.GPID,4);
//	GP_TextButton* a_ptbNameNew = a_dsMenu.addGP_TextButton(NULL, bX0+9,bY0+8 ,GetTextByID("#CIB_NEW"),a_lgpButtons.GPID, 6 ,&WhiteFont,&YellowFont);//NEED NEW FONTS 
//	//a_ptbNameNew->DisabledFont  = &BlackFont;	
//	a_ptbNameNew->OnUserClick   = &OnPIButtonClicked; //&OnNewPlayerClicked; 
//	a_ptbNameNew->UserParam	    = 0;
//	
//	a_dsMenu.addGPPicture(NULL, bX0+224,bY0 ,a_lgpButtons.GPID,4);
//	GP_TextButton* a_ptbNameDel = a_dsMenu.addGP_TextButton(NULL, bX0+224+9,bY0+8 ,GetTextByID("#CIB_DELETE"),a_lgpButtons.GPID, 6 ,&WhiteFont,&YellowFont);//NEED NEW FONTS 
//	//a_ptbNameDel->DisabledFont  = &BlackFont;		
//	a_ptbNameDel->OnUserClick   = &OnPIButtonClicked; //&OnDeletePlayerClicked; 
//	a_ptbNameDel->UserParam	    = 0;
//	
//	a_dsMenu.addGPPicture(NULL, bX0,bY0+42 ,a_lgpButtons.GPID,4);
//	GP_TextButton* a_ptbNameOk = a_dsMenu.addGP_TextButton(NULL, bX0+9,bY0+42+8 ,GetTextByID("#CDB_OkButton"),a_lgpButtons.GPID, 6 ,&WhiteFont,&YellowFont);//NEED NEW FONTS 
//	//a_ptbNameOk->DisabledFont  = &BlackFont;		
//	a_ptbNameOk->OnUserClick   = &MMItemChoose; 
//	a_ptbNameOk->UserParam	   = mcmOk;
//	
//	a_dsMenu.addGPPicture(NULL, bX0+224,bY0+42 ,a_lgpButtons.GPID,4);
//	GP_TextButton* a_ptbNameCancel = a_dsMenu.addGP_TextButton(NULL, bX0+224+9,bY0+42+8 ,GetTextByID("#CDB_Cancel"),a_lgpButtons.GPID, 6 ,&WhiteFont,&YellowFont);//NEED NEW FONTS 
//	//a_ptbNameCancel->DisabledFont  = &BlackFont;		
//	a_ptbNameCancel->OnUserClick   = &MMItemChoose; 
//	a_ptbNameCancel->UserParam	   = mcmCancel;   
//
//
//	//обработка экрана
//	ItemChoose	= - 1;
//
//	do 
//	{
//		ProcessMessages();
//		StdKeys();
//		a_dsMenu.ProcessDialogs();
//		a_dsMenu.RefreshView();
//
//
//		//тушим отжатую кнопку и запоминаем нажатую
//		for(int i=0; i<Names; i++) 
//			if ( a_ptbName[i]->UserParam && i!=Active )		
//			{
//				if (Active!=-1) {
//					a_ptbName[Active]->Sprite    = 6 ;				//active
//					a_ptbName[Active]->Sprite1   = 4 ;				//passive
//					a_ptbName[Active]->ActiveFont  = &WhiteFont; //NEED NEW FONTS
//					a_ptbName[Active]->PassiveFont = &YellowFont; //NEED NEW FONTS
//					a_ptbName[Active]->UserParam = 0;				//off
//				}
//				else			{ 
//					a_ptbNameOk->Enabled = true; a_ptbNameDel->Enabled = true; 
//				}
//				
//				Active = i;											//запоминаем активное имя (нажатую кнопку)
//			} //end of pressed buttons
//
//		
//		//добавление нового игрока (имени)
//		if(a_ptbNameNew->UserParam)	{
//			if(Names!=MAXNAMES) 
//			{ 
//				a_ptbNameNew->Enabled = false; a_ptbNameDel->Enabled = false; a_ptbNameOk->Enabled = false; a_ptbNameCancel->Enabled = false; a_pddDesk->Enabled = false; for(int i=0; i<Names; i++) { a_ptbName[i]->Enabled = false;; }
//
//				const int Len = 31;
//				char* newName;
//				newName = znew(char, Len+1);
//
//                int	  heroId = 0;
//				if(PlayerIntrfAddNewPlayer(&a_dsMenu, &ActiveNation, &ActiveDiff, &newName, &heroId )) //добавление нового игрока
//				{
//					//new save
//					int id = SAVE.GetNextId(); 
//					CPlayerSAVE_STRUCT* SS = new CPlayerSAVE_STRUCT;  
//					SAVE.m_pPlayer.Add(SS); 
//					SAVE.m_pPlayer[Names]->m_iId         = id;
//					SAVE.m_pPlayer[Names]->m_chName      = newName;	
//					SAVE.m_pPlayer[Names]->m_iNation     = ActiveNation;	
//					SAVE.m_pPlayer[Names]->m_iDifficulty = ActiveDiff;
//					SAVE.m_pPlayer[Names]->m_iCurHeroId  = heroId; //m_iFirstHeroId = heroId; //damn new
//					//SAVE.m_pPlayer[0].m_iNation     = ActiveNation;	//SAVE.m_pPlayer[0].m_iDifficulty = ActiveDiff;	//SAVE.m_pPlayer[0].m_iFirstHeroId     = heroId;	//SAVE.m_pPlayer[0].m_chName.pchar() = znew(char,strlen(NewName)+1);	//strcpy( SAVE.m_pPlayer[0].m_chName.pchar(), NewName );
//
//					for(int i=Names; i>0; i--) { 
//						a_ptbName[i]	  = a_ptbName[i-1]; 
//						a_pgppFlag[i]     = a_pgppFlag[i-1];
//						a_pgppColor[i]    = a_pgppColor[i-1];
//
//						a_ptbName[i]->y   += 35;	a_ptbName[i]->y1   += 35;
//						a_pgppFlag[i]->y  += 35;	a_pgppFlag[i]->y1  += 35;
//						a_pgppColor[i]->y += 35;	a_pgppColor[i]->y1 += 35;
//
//						//move new save UP	
//						SAVE.m_pPlayer.MoveElement(i, -1);
//					}
//					if(Active!=-1) Active++; 
//					Names++;
//
//					//имя нового игрока
//					a_ptbName[0] = a_pddDesk->addGP_TextButton(NULL,dX0,dY0,newName,a_lgpNameButton.GPID,4/*0*/,&WhiteFont,&YellowFont); //NEED NEW FONTS
//					a_ptbName[0]->Center    = false;
//					a_ptbName[0]->FontDy    = -3; 
//					a_ptbName[0]->OnClick   = &OnPlayerNameClicked;
//					OnPlayerNameClicked(a_ptbName[0]);
//					//флаг игрока
//					a_pgppFlag[0] = a_pddDesk->addGPPicture(NULL, a_ptbName[0]->x+300,a_ptbName[0]->y-5 ,a_lgpFlags.GPID, ActiveNation+1 );
//					//цвет игрока
//					a_pgppColor[0] = a_pddDesk->addGPPicture(NULL, a_ptbName[0]->x+350,a_ptbName[0]->y+3 ,a_lColorValue.GPID, ActiveNation );
//					a_pddDesk->VScroller->SPos = 0; //scrolling - to upper position
//				}
//				
//			delete[]newName; 
//
//			a_ptbNameNew->Enabled = true;  a_ptbNameDel->Enabled = true;  a_ptbNameOk->Enabled = true;  a_ptbNameCancel->Enabled = true;  a_pddDesk->Enabled = true;  for(int i=0; i<Names; i++) { a_ptbName[i]->Enabled = true; }
//			}
//
//			a_ptbNameNew->UserParam=0;  
//		} // end of Add New Player
//
//
//		//удаление игрока (имени)
//		if(a_ptbNameDel->UserParam)	{ 
//			a_ptbNameNew->Enabled = false; a_ptbNameDel->Enabled = false; a_ptbNameOk->Enabled = false; a_ptbNameCancel->Enabled = false; a_pddDesk->Enabled = false; for(int i=0; i<Names; i++) { a_ptbName[i]->Enabled = false;; }
//
//			if(PlayerIntrfDeletePlayer(&a_dsMenu, SAVE.m_pPlayer[Active]->m_chName.pchar(), SAVE.m_pPlayer[Active]->m_iNation)) 
//			{
//				SAVE.m_pPlayer.DelElement(Active);
//
//				a_ptbName[Active]->y = a_pgppFlag[Active]->y = a_pgppColor[Active]->y = dY0;
//				a_ptbName[Active]->Enabled = a_pgppFlag[Active]->Enabled = a_pgppColor[Active]->Enabled = false;
//				a_ptbName[Active]->Visible = a_pgppFlag[Active]->Visible = a_pgppColor[Active]->Visible = false;
//
//				for(int i=Active; i<Names-1; i++) { 
//						a_ptbName[i]	  = a_ptbName[i+1]; 
//						a_pgppFlag[i]     = a_pgppFlag[i+1];
//						a_pgppColor[i]    = a_pgppColor[i+1];
//
//						a_ptbName[i]->y   -= 35;	a_ptbName[i]->y1   -= 35;
//						a_pgppFlag[i]->y  -= 35;	a_pgppFlag[i]->y1  -= 35;
//						a_pgppColor[i]->y -= 35;	a_pgppColor[i]->y1 -= 35;
//				}
//				Names--;
//				if(Active>=Names) Active--; 
//
//				if(Active>=0) OnPlayerNameClicked(a_ptbName[Active]);
//			}
//
//			a_ptbNameDel->UserParam=0;  
//			a_ptbNameNew->Enabled = true;  a_ptbNameDel->Enabled = true;  a_ptbNameOk->Enabled = true;  a_ptbNameCancel->Enabled = true;  a_pddDesk->Enabled = true;  for(int i=0; i<Names; i++) { a_ptbName[i]->Enabled = true; }
//		} // end of Delete Player
//
//		
//		//check enabled buttons
//		if(Active<0) {    
//			a_ptbNameOk->Enabled  = false;  
//			a_ptbNameDel->Enabled = false;  
//			if(ItemChoose==mcmOk) { ItemChoose = -1; }   
//		}
//
//	}
//	while (ItemChoose!=mcmOk && ItemChoose!=mcmCancel);
//	
//
//	//for save Active name FIRST
//	if(Active>0&&ItemChoose==mcmOk) {
//		for(int i=Active; i>0; i--) { SAVE.m_pPlayer.MoveElement(i, -1); }		
//	}
//	
//	////saving to XML			/* SAVE "bmplsav.xml" (BigMap Players Save) */
//	//xmlQuote xml2;
//	//SAVE.Save(xml2,&SAVE);
//	//xml2.WriteToFile("Players\\bmplsav.xml"); 
//	SAVE.SaveXML();
//	
//	if (ItemChoose == mcmCancel) { ItemChoose = -1; return false; }
//
//	ItemChoose = -1;
//
//	return true;
//}
//
////-------------------------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------------------------
//
//
////// меню добавления нового игрока
//bool	PlayerIntrfAddNewPlayer(DialogsSystem* pMenu, int* IntrfActiveNation, int* IntrfActiveDiff, char** name, int* heroId) //bool	PlayerIntrfAddNewPlayer(DialogsSystem* pMenu, CPlayer_SAVE_STRUCT* pPlayer, int* IntrfActiveNation, int* IntrfActiveDiff)
//{
//	int X0 = 470;			//координатa X0
//	int Y0 = 260;			//координатa Y0
//
//	int ActiveNation = *IntrfActiveNation;
//	int ActiveDiff   = *IntrfActiveDiff;
//	int	ActiveHero[6] = { 0, 0, 0, 0, 0, 0 };
//		
//	const int iPlNameLen = 31;
//
//	DialogsSystem	a_dsMenu(0,0);				//новое меню
//
//	//BlackScreen
//	LocalGP		a_lgpScreen("Interf3\\TotalWarGraph\\blackscr"); //затемнение экрана			
//	GPPicture*	a_pgppFadeScr = a_dsMenu.addGPPicture(NULL,0,0,a_lgpScreen.GPID,0);
//	a_pgppFadeScr->Diffuse = 0x88202020;
//
//	// WINDOW BLOCK
//	//New Player box
//	LocalGP		a_lgpNewPlayer("Interf3\\TotalWarGraph\\int_NewPlayer"); //			
//	GPPicture*	a_pgppNewPlBox = a_dsMenu.addGPPicture(NULL, 370 /*X0*/, 190 /*Y0*/,a_lgpNewPlayer.GPID,0);
//	a_pgppNewPlBox->OnUserClick     = &EmptyClick; 
//
//	//box head
//	TextButton*	a_ptbTHead = a_dsMenu.addTextButton(NULL,X0+205/*245*/,Y0+(-47),GetTextByID("#CIT_NewPlayer"),&YellowFont,&YellowFont,&YellowFont,1);		
//
//	// PLAYERS BLOCK
//	//enter player name
//	TextButton*	a_ptbTName = a_dsMenu.addTextButton(NULL, X0,Y0 ,GetTextByID("#CIT_EnterPlayerName"),&BlackFont,&BlackFont,&BlackFont,0);		
//	a_ptbTHead->Enabled = false;
//
//	//player name InputBox
//	int ibWidth  = 150; //320; 
//	int ibHeight =  22; 
//	LocalGP		a_lgpibPlayerName("Interf3\\TotalWarGraph\\int_ibPlayername"); //			
//	GP_TextButton*	a_pgppibGPTName = a_dsMenu.addGP_TextButton(NULL, a_ptbTName->x+200,a_ptbTName->y ,"",a_lgpibPlayerName.GPID, 4, &YellowFont,&YellowFont);
//	a_pgppibGPTName->Sprite1 = 4; //passive
//	a_pgppibGPTName->x1 = a_pgppibGPTName->x + ibWidth ; //- 25;	
//	//a_pgppibGPTName->y1 = a_pgppibGPTName->y + ibHeight;
//	char PlayerName[iPlNameLen]="";
//	InputBox* a_pibName=a_dsMenu.addInputBox(NULL, a_pgppibGPTName->x+10,a_pgppibGPTName->y+0 ,*name/*PlayerName*/,iPlNameLen/*250*/,ibWidth/*320*/,ibHeight/*22*/,&YellowFont,&YellowFont, 0);  //NEED NEW FONTS
//	a_pibName->x1 = a_pgppibGPTName->x1 ; //- 10;	
//	a_pibName->y1 = a_pgppibGPTName->y1 ;	
//	a_pibName->Active = true;
//
//	//enter player nation
//	TextButton*	a_ptbTNation = a_dsMenu.addTextButton(NULL, X0,Y0+30 ,GetTextByID("#CIT_EnterPlayerNat"),&BlackFont,&BlackFont,&BlackFont,0);		
//	a_ptbTNation->Enabled = false;
//	//player nation combobox
//	LocalGP		a_lgpComboBox("Interface\\CompCombo"); //("Interf2\\Combo");
//	ComboBox*	a_pcbNation = a_dsMenu.addGP_ComboBox(NULL, a_ptbTNation->x+200,a_ptbTNation->y ,a_lgpComboBox.GPID,0,9,0,&BlackFont,&BlackFont,NULL); //&YellowFont,&YellowFont,NULL);
//	a_pcbNation->CurLine = ActiveNation;
//	a_pcbNation->Center-=5;
//	a_pcbNation->FontDy-=2;
//	a_pcbNation->OneDy-=1;
//
//	a_pcbNation->AddLine( GetTextByID( "#ENGLAND" ) );
//	a_pcbNation->AddLine( GetTextByID( "#FRANCE"  ) );
//	//DEMO
//	//a_pcbNation->AddLine( GetTextByID( "#AUSTRIA" ) );
//	//a_pcbNation->AddLine( GetTextByID( "#PRUSSIA" ) );
//	//a_pcbNation->AddLine( GetTextByID( "#RUSSIA"  ) );
//	//a_pcbNation->AddLine( GetTextByID( "#EGYPT"   ) );
//	//a_pcbNation->Hint=GetTextByID("NATIONS");
//	//DEMO
//
//	//enter difficulty level
//	TextButton*	a_ptbTDiff = a_dsMenu.addTextButton(NULL, X0,Y0+70 ,GetTextByID("#CIT_EnterDifficulty"),&BlackFont,&BlackFont,&BlackFont,0);		
//	a_ptbTDiff->Enabled = false;
//	
//	//difficulty combobox
//	ComboBox*	a_pcbDiff=a_dsMenu.addGP_ComboBox(NULL, a_ptbTDiff->x+200,a_ptbTDiff->y ,a_lgpComboBox.GPID,0,9,0,&BlackFont,&BlackFont,NULL); //&YellowFont,&YellowFont,NULL);
//	a_pcbDiff->CurLine = ActiveDiff;
//	a_pcbDiff->Center-=5;
//	a_pcbDiff->FontDy-=2;
//	a_pcbDiff->OneDy-=1;
//
//	//if(DIFF_LEVELS_N > 1) a_pcbDiff->AddLine(GetTextByID("@RMID_EASY"));
//	a_pcbDiff->AddLine(GetTextByID("@RMID_NORMAL"));
//	if(DIFF_LEVELS_N > 2) a_pcbDiff->AddLine(GetTextByID("@RMID_HARD"));
//	//if(DIFF_LEVELS_N > 3) a_pcbDiff->AddLine(GetTextByID("@RMID_VERYHARD"));
//	//if(DIFF_LEVELS_N > 4) a_pcbDiff->Hint=GetTextByID("DIFFICULT5");
//
//	//player color
//	LocalGP		a_lColorValue("Interf2\\PlayerList");	//color
//	GPPicture*	a_pgppColor = a_dsMenu.addGPPicture(NULL, a_ptbTName->x+400,a_ptbTName->y ,a_lColorValue.GPID, ActiveNation ); 
//
//	//player national flag
//	LocalGP		a_lgpFlags("Interf3\\TotalWarGraph\\lva_Flags");	//флаги
//	GPPicture*	a_pgppFlag = a_dsMenu.addGPPicture(NULL, a_ptbTNation->x+400,a_ptbTNation->y ,a_lgpFlags.GPID, ActiveNation + 1 ); 
//	
//	// HERO'S BLOCK
//	//hero head
//	TextButton*	a_ptbTHeroHead = a_dsMenu.addTextButton(NULL, X0+205,Y0+110 ,GetTextByID("#CIT_PlayersHero"),&BlackFont,&BlackFont,&BlackFont, 1 );		
//
//	//heroes portraites
//	LocalGP		lgpPORTRAIT_EGs( "Interf3\\TotalWarGraph\\lva_EGs" );
//	LocalGP		lgpPORTRAIT_FRs( "Interf3\\TotalWarGraph\\lva_FRs" );
//	LocalGP		lgpPORTRAIT_ASs( "Interf3\\TotalWarGraph\\lva_ASs" );
//	LocalGP		lgpPORTRAIT_PRs( "Interf3\\TotalWarGraph\\lva_PRs" );
//	LocalGP		lgpPORTRAIT_RSs( "Interf3\\TotalWarGraph\\lva_RSs" );
//	LocalGP		lgpPORTRAIT_EPs( "Interf3\\TotalWarGraph\\lva_EPs" );
//
//	//hero portrait
//	GPPicture*	a_pgppHero = a_dsMenu.addGPPicture(NULL, X0,Y0+150 ,lgpPORTRAIT_EGs.GPID, 0 );
//
//	//hero name
//	TextButton*	a_ptbTHeroName = a_dsMenu.addTextButton(NULL, a_pgppHero->x+300,a_pgppHero->y ,"",&BlackFont,&BlackFont,&BlackFont, 1 ); 
//	
//	Enumerator*	HeroName_ENUM = ENUM.Get("HeroNames"); // Enumerator for HeroNames 
//	if (!HeroName_ENUM->GetAmount()) { 
//		char s[256];
//		int  maxH = MAX_HEROES;
//		int  id = 0;
//		for(int i=0; i<maxH; i++) {			// 0
//			sprintf(s, "#HERO_EG_%d", i );
//			HeroName_ENUM->Add( GetTextByID(s), id++ );		}
//		for(int i=0; i<maxH; i++) {			// 1
//			sprintf(s, "#HERO_FR_%d", i );
//			HeroName_ENUM->Add( GetTextByID(s), id++ );		}
//		for(int i=0; i<maxH; i++) {			// 2
//			sprintf(s, "#HERO_AS_%d", i );
//			HeroName_ENUM->Add( GetTextByID(s), id++ );		}
//		for(int i=0; i<maxH; i++) {			// 3
//			sprintf(s, "#HERO_PR_%d", i );
//			HeroName_ENUM->Add( GetTextByID(s), id++ );		}
//		for(int i=0; i<maxH; i++) {			// 4
//			sprintf(s, "#HERO_RS_%d", i );
//			HeroName_ENUM->Add( GetTextByID(s), id++ );		}
//		for(int i=0; i<maxH; i++) {			// 5
//			sprintf(s, "#HERO_EP_%d", i );
//			HeroName_ENUM->Add( GetTextByID(s), id++ );		}
//	} //TEMP!!!
//
//	//hero descriptions
//	TextButton* a_ptbTHeroDescr[5]; 
//	for(int i=0; i<5; i++)
//		a_ptbTHeroDescr[i] = a_dsMenu.addTextButton(NULL,a_ptbTName->x+200,a_ptbTHeroName->y+20*(i+1),"Hero Description",&BlackFont,&BlackFont,&BlackFont,0);//NEED NEW FONTS  
//	//Задать энумератор для рангов героев.
//	Enumerator*	RANK_ENUM = ENUM.Get("RANK_ENUM");
//
//	//hero RANK name
//	int iDesc = 0;
//	char* str = RANK_ENUM->Get(iDesc);
//	delete[]a_ptbTHeroDescr[iDesc]->Message;
//	a_ptbTHeroDescr[iDesc]->Message=znew(char,strlen(str)+1);
//	strcpy(a_ptbTHeroDescr[iDesc]->Message,str);				//hero rank
//
//	//if (!RANK_ENUM->Values.GetAmount()) { //temp
//	//	RANK_ENUM->Add( GetTextByID( "#LIEUTENANT"	),	0 );
//	//	RANK_ENUM->Add( GetTextByID( "#CAPTAIN"		),	1 );
//	//	RANK_ENUM->Add( GetTextByID( "#MAJOR"		),	2 );
//	//	RANK_ENUM->Add( GetTextByID( "#COLONEL"		),	3 );
//	//	RANK_ENUM->Add( GetTextByID( "#GENERAL"		),	4 );
//	//	RANK_ENUM->Add( GetTextByID( "#MARSHAL"		),	5 );
//	//}
//
//	//hero prev/next buttons
//	LocalGP		a_lgpButtons("Interf3\\TotalWarGraph\\dMessage");
//	a_dsMenu.addGPPicture(NULL, X0+149,Y0+300 ,a_lgpButtons.GPID, 5 );
//	GP_TextButton* a_ptbPrev = a_dsMenu.addGP_TextButton(NULL, X0+149+9,Y0+300+8 ,GetTextByID("#CIB_Prev"),a_lgpButtons.GPID, 7 ,&WhiteFont,&YellowFont);//NEED NEW FONTS 
//	//a_ptbPrev->DisabledFont  = &BlackFont;		
//	a_ptbPrev->OnUserClick   = &OnPIButtonClicked; //&OnNextHeroClicked; 
//	a_ptbPrev->UserParam	 = 0;
//	
//	a_dsMenu.addGPPicture(NULL, X0+300,Y0+300 ,a_lgpButtons.GPID, 5 );
//	GP_TextButton* a_ptbNext = a_dsMenu.addGP_TextButton(NULL, X0+300+9,Y0+300+8 ,GetTextByID("#CIB_Next"),a_lgpButtons.GPID, 7 ,&WhiteFont,&YellowFont);//NEED NEW FONTS 
//	//a_ptbNext->DisabledFont  = &BlackFont;		
//	a_ptbNext->OnUserClick   = &OnPIButtonClicked; //&OnNextHeroClicked; 
//	a_ptbNext->UserParam	 = 0;   
//
//	// OK/CANCEL BUTTONS BLOCK
//	int bX0 = X0 + 10;			//X0 += 50; 
//	int bY0 = Y0 + 310;			//260 + 50 ;	//240;
//	a_dsMenu.addGPPicture(NULL, bX0,bY0+42 ,a_lgpButtons.GPID,4);
//	GP_TextButton* a_ptbNameOk = a_dsMenu.addGP_TextButton(NULL, bX0+9,bY0+42+8 ,GetTextByID("#CDB_OkButton"),a_lgpButtons.GPID, 6 ,&WhiteFont,&YellowFont);//NEED NEW FONTS 
//	//a_ptbNameOk->DisabledFont  = &BlackFont;		
//	a_ptbNameOk->OnUserClick   = &MMItemChoose; 
//	a_ptbNameOk->UserParam	   = mcmOk;
//	a_ptbNameOk->Enabled       = false;
//	
//	a_dsMenu.addGPPicture(NULL, bX0+224,bY0+42 ,a_lgpButtons.GPID,4);
//	GP_TextButton* a_ptbNameCancel = a_dsMenu.addGP_TextButton(NULL, bX0+224+9,bY0+42+8 ,GetTextByID("#CDB_Cancel"),a_lgpButtons.GPID, 6 ,&WhiteFont,&YellowFont);//NEED NEW FONTS 
//	//a_ptbNameCancel->DisabledFont  = &BlackFont;		
//	a_ptbNameCancel->OnUserClick   = &MMItemChoose; 
//	a_ptbNameCancel->UserParam	   = mcmCancel;   
//
//
//	ItemChoose   = -1;
//	ActiveNation = -1;
//	ActiveDiff	 = -1;
//
//	do 
//	{
//		ProcessMessages();
//		StdKeys();
//		pMenu->ProcessDialogs();
//		a_dsMenu.ProcessDialogs();
//		a_dsMenu.RefreshView();
//
//
//		//if nation is changed
//		if(a_pcbNation->CurLine != ActiveNation) 
//		{
//			ActiveNation = a_pcbNation->CurLine;
//			a_pgppColor->SpriteID = ActiveNation;
//			a_pgppFlag->SpriteID  = ActiveNation+1;
//			
//			switch (ActiveNation) {
//				case	0:
//					a_pgppHero->FileID = lgpPORTRAIT_EGs.GPID;
//					break;
//				case	1:
//					a_pgppHero->FileID = lgpPORTRAIT_FRs.GPID;
//					break;
//				case	2:
//					a_pgppHero->FileID = lgpPORTRAIT_ASs.GPID;
//					break;
//				case	3:
//					a_pgppHero->FileID = lgpPORTRAIT_PRs.GPID;
//					break;
//				case	4:
//					a_pgppHero->FileID = lgpPORTRAIT_RSs.GPID;
//					break;
//				case	5:
//					a_pgppHero->FileID = lgpPORTRAIT_EPs.GPID;	
//			}
//
//			a_pgppHero->SpriteID = ActiveHero[ ActiveNation ];
//
//			char* str = HeroName_ENUM->Get( ActiveHero[ ActiveNation ] + ActiveNation*MAX_HEROES );
//			delete[]a_ptbTHeroName->Message;
//			a_ptbTHeroName->Message=znew(char,strlen(str)+1);
//			strcpy(a_ptbTHeroName->Message,str);	
//		}//end if (nation)
//
//
//		//if difficulty is changed
//		if(a_pcbDiff->CurLine != ActiveDiff)
//		{
//			ActiveDiff = a_pcbDiff->CurLine;
//			//char* str = RANK_ENUM->Get(DIFF_LEVELS_N-ActiveDiff-1);
//			//delete[]a_ptbTHeroDescr[0]->Message;
//			//a_ptbTHeroDescr[0]->Message=znew(char,strlen(str)+1);
//			//strcpy(a_ptbTHeroDescr[0]->Message,str);				//hero rank
//		}
//		//end if (difficulty)
//
//
//		//if hero is changed   
//		if(a_ptbPrev->UserParam)	 
//		{ 
//			if(ActiveHero[ ActiveNation ] > 0)	{ ActiveHero[ActiveNation]--; }
//			ActiveNation = -1;
//			a_ptbPrev->UserParam=0;  
//		}
//
//		if(a_ptbNext->UserParam)	
//		{ 
//			if(ActiveHero[ ActiveNation ] < MAX_HEROES-1)	{ ActiveHero[ActiveNation]++; }
//			ActiveNation = -1;
//			a_ptbNext->UserParam=0;  
//		}//end if (hero)
//
//		
//		//check enabled buttons
//		a_ptbNameOk->Enabled = strlen(*name)!=0;
//	//DEMO
//		a_ptbPrev->Enabled = false ;
//		a_ptbNext->Enabled = false ;
//		//a_ptbPrev->Enabled = ActiveHero[ ActiveNation ]!=0;
//		//a_ptbNext->Enabled = ActiveHero[ ActiveNation ]!=9;
//	//DEMO
//		if(ItemChoose==mcmOk&&strlen(*name)==0) { ItemChoose = -1; }
//
//	}
//	while (ItemChoose!=mcmOk && ItemChoose!=mcmCancel);
//
//
//	*IntrfActiveNation = ActiveNation;
//	*IntrfActiveDiff   = ActiveDiff;
//
//	if(ItemChoose==mcmCancel) {	ItemChoose = -1; return false; }
//
//	*heroId = ActiveHero[ ActiveNation ];
//
//	ItemChoose = -1;
//
//	return true;
//}
//
////-------------------------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------------------------
//
//
////// DELETE existing Player Name
//bool	PlayerIntrfDeletePlayer(DialogsSystem* pMenu, char* name, int nation)
//{
//	int X0 = 370; //420 + 50 + 205;	//координатa X0
//	int Y0 = 190; //260;			//координатa Y0
//
//	DialogsSystem	a_dsMenu(0,0);				//новое меню
//
//	//BlackScreen
//	LocalGP		a_lgpDelScr("Interf3\\TotalWarGraph\\blackscr"); //затемнение экрана			
//	GPPicture*	a_pgppDelScr = a_dsMenu.addGPPicture(NULL,0,0,a_lgpDelScr.GPID,0);
//	a_pgppDelScr->Diffuse = 0x88202020; 
//
//	// WINDOW BLOCK
//	//Delete Player box
//	LocalGP		a_lgpDelPlayer("Interf3\\TotalWarGraph\\int_NewPlayer"); //			
//	GPPicture*	a_pgppDelPlBox = a_dsMenu.addGPPicture(NULL, X0, Y0,a_lgpDelPlayer.GPID,0);
//
//	//box head
//	TextButton*	a_ptbTHead = a_dsMenu.addTextButton(NULL,X0+308/*245*/,Y0+23/*(-47)*/,GetTextByID("#CIT_DelPlayer"),&YellowFont,&YellowFont,&YellowFont,1);		
//
//	// PLAYERS BLOCK
//	//player name
//	LocalGP		    a_lgpNameButton("Interf3\\TotalWarGraph\\int_PlayerName");	//фон для имен игроков
//	GP_TextButton*  a_ptbTName = a_dsMenu.addGP_TextButton(NULL, X0+308/*245*/,Y0+80/*10*/ ,name,a_lgpNameButton.GPID,4,&YellowFont,&YellowFont);//NEED NEW FONTS 
//	int nameLen = GPS.GetGPWidth(a_lgpNameButton.GPID, a_ptbTName->Sprite );
//	a_ptbTName->Sprite1 = 4; //passive
//	a_ptbTName->x  = a_ptbTName->x - nameLen/2;	
//	a_ptbTName->x1 = a_ptbTName->x + nameLen - 1;
//
//	//player national flag
//	LocalGP		a_lgpFlags("Interf3\\TotalWarGraph\\lva_Flags");			//флаги
//	GPPicture*	a_pgppFlag = a_dsMenu.addGPPicture(NULL, X0+308,Y0+120/*50*/ ,a_lgpFlags.GPID,nation+1); 
//	int flagLen = GPS.GetGPWidth(a_lgpFlags.GPID,0);
//	a_pgppFlag->x  = a_pgppFlag->x - flagLen/2;
//	a_pgppFlag->x1 = a_pgppFlag->x + flagLen - 1;
//
//	// YES/NO BUTTONS BLOCK
//	int bX0 = X0 +  80;			//X0 += 50; 
//	int bY0 = Y0 + 380;			//240;
//
//	LocalGP		a_lgpButtons("Interf3\\TotalWarGraph\\dMessage");
//
//	a_dsMenu.addGPPicture(NULL, bX0,bY0+42 ,a_lgpButtons.GPID,4);
//	GP_TextButton* a_ptbNameOk = a_dsMenu.addGP_TextButton(NULL, bX0+9,bY0+42+8 ,GetTextByID("#CDB_YesButton"),a_lgpButtons.GPID, 6 ,&WhiteFont,&YellowFont);//NEED NEW FONTS 
//	a_ptbNameOk->OnUserClick   = &MMItemChoose; 
//	a_ptbNameOk->UserParam	   = mcmOk;
//	
//	a_dsMenu.addGPPicture(NULL, bX0+224,bY0+42 ,a_lgpButtons.GPID,4);
//	GP_TextButton* a_ptbNameCancel = a_dsMenu.addGP_TextButton(NULL, bX0+224+9,bY0+42+8 ,GetTextByID("#CDB_NoButton"),a_lgpButtons.GPID, 6 ,&WhiteFont,&YellowFont);//NEED NEW FONTS 
//	a_ptbNameCancel->OnUserClick   = &MMItemChoose; 
//	a_ptbNameCancel->UserParam	   = mcmCancel;   
//
//
//	ItemChoose	= -1;
//
//	do {
//		ProcessMessages();
//		StdKeys();
//		pMenu->ProcessDialogs();
//		a_dsMenu.ProcessDialogs();
//		a_dsMenu.RefreshView();
//	}
//	while (ItemChoose!=mcmOk && ItemChoose!=mcmCancel);
//
//	if(ItemChoose==mcmCancel) {	ItemChoose = -1; return false; }
//
//	ItemChoose = -1;
//
//	return true;
//}
////-------------------------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------------------------
//
//
//
////-------------------------------------------------------------------------------------------------------------------
////---------------   OnCLICK functions   -----------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------------------------
//bool	OnPlayerNameClicked(SimpleDialog* pSD) {
//	if(!pSD)  return false;
//	GP_TextButton* a_ptbName = reinterpret_cast<GP_TextButton*>(pSD);
//	a_ptbName->Sprite  = 9 ; //2; //active
//	a_ptbName->Sprite1 = 9 ; //2; //passive
//	a_ptbName->ActiveFont  = &WhiteFont; //&RedFont; //NEED NEW FONTS
//	a_ptbName->PassiveFont = &WhiteFont; //&RedFont; //NEED NEW FONTS
//	a_ptbName->UserParam = 1;	//активна сейчас
//	return true;
//}
////-------------------------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------------------------
//
//
////-------------------------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------------------------
////*******************************************************************************************************************
////*******************************************************************************************************************
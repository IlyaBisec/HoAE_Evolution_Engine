#define INT_SHIFT 8
#define dint fint<Shift>
#define __temp template <int Shift>
__temp class fint{
protected:
	int Value;
public:
	__forceinline fint(int v){
		Value=1<<Shift;
	}
	__forceinline fint(){
		Value=0;
	}
	friend __forceinline dint operator + (fint v1,fint v2);
	friend __forceinline dint operator + (dint v1,int v2);
	friend __forceinline dint operator + (int v2,dint v1);
	friend __forceinline dint operator + (dint v1,float v2);
	friend __forceinline dint operator + (float v2,dint v1);

	friend __forceinline dint operator - (fint v1,fint v2);
	friend __forceinline dint operator - (dint v1,int v2);
	friend __forceinline dint operator - (int v2,dint v1);
	friend __forceinline dint operator - (dint v1,float v2);
	friend __forceinline dint operator - (float v2,dint v1);

	friend __forceinline dint operator * (fint v1,fint v2);
	friend __forceinline dint operator * (dint v1,int v2);
	friend __forceinline dint operator * (int v2,dint v1);
	friend __forceinline dint operator * (dint v1,float v2);
	friend __forceinline dint operator * (float v2,dint v1);

	friend __forceinline dint operator / (fint v1,fint v2);
	friend __forceinline dint operator / (dint v1,int v2);
	friend __forceinline dint operator / (int v2,dint v1);
	friend __forceinline dint operator / (dint v1,float v2);
	friend __forceinline dint operator / (float v2,dint v1);
	
	__forceinline fint& operator - (){
        Value=-Value;
		return *this;
	};

	__forceinline dint operator = (const int& v){
		Value=v<<Shift;
	}
	__forceinline dint operator = (const float& v){
		Value=int(v*(1<<Shift));
	}
};

__temp __forceinline dint operator + (dint v1,dint v2){
	dint result;
	result.Value=v1.Value+v2.Value;
	return result;
}
__temp __forceinline dint operator + (dint v1,int v2){
	dint result;
	result.Value=v1.Value+(v2<<Shift);
	return result;
}
__temp __forceinline dint operator + (dint v1,float v2){
	dint result;
	result.Value=v1.Value+int(v2*(1<<Shift));
	return result;
}
__temp __forceinline dint operator + (int v2,dint v1){
	dint result;
	result.Value=v1.Value+(v2<<Shift);
	return result;
}
__temp __forceinline dint operator + (float v2,dint v1){
	dint result;
	result.Value=v1.Value+int(v2*(1<<Shift));
	return result;
}


__temp __forceinline dint operator - (dint v1,dint v2){
	dint result;
	result.Value=v1.Value-v2.Value;
	return result;
}
__temp __forceinline dint operator - (dint v1,int v2){
	dint result;
	result.Value=v1.Value-(v2<<Shift);
	return result;
}
__temp __forceinline dint operator - (dint v1,float v2){
	dint result;
	result.Value=v1.Value-int(v2*(1<<Shift));
	return result;
}
__temp __forceinline dint operator - (int v2,dint v1){
	dint result;
	result.Value=v1.Value-(v2<<Shift);
	return result;
}
__temp __forceinline dint operator - (float v2,dint v1){
	dint result;
	result.Value=v1.Value-int(v2*(1<<Shift));
	return result;
}


__temp __forceinline dint operator / (dint v1,dint v2){
	dint result;
	result.Value=(v1.Value<<Shift)/(v2.Value>>Shift);
	return result;
}
__temp __forceinline dint operator / (dint v1,int v2){
	dint result;
	result.Value=v1/v2;
	return result;
}
__temp __forceinline dint operator / (dint v1,float v2){
	dint result;
	result.Value=v1/dint(v2);
	return result;
}
__temp __forceinline dint operator / (int v2,dint v1){
	dint result;
	result.Value=dint(v2)/v1;
	return result;
}
__temp __forceinline dint operator / (float v2,dint v1){
	dint result;
	result.Value=dint(v2)/v1;
	return result;
}

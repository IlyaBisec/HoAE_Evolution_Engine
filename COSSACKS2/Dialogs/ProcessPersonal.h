//-------------------------------------------------------------------------------------------
//Персональное меню игрока.
//-------------------------------------------------------------------------------------------

bool	ProcessPersonal(int n)
{
	KeyPressed  = 0;        //Global
    LastKey     = 0;        //Global

	DialogsSystem   a_dsMenu(0,0);
	SQPicture   a_sqpBack("Interf2\\PersonalFon.bmp"); //Фон
	a_dsMenu.addPicture(NULL,0,0,&a_sqpBack,&a_sqpBack,&a_sqpBack);

	ItemChoose	= - 1;
	do {
		ProcessMessages();
		StdKeys();
		a_dsMenu.ProcessDialogs();
		a_dsMenu.RefreshView();
	} 
	while (ItemChoose!=mcmOk && ItemChoose!=mcmCancel);

	return true;
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
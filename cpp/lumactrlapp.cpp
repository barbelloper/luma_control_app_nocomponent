#include <stdio.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include "luma_ctrl.h"

using namespace std;

int command_parser(int argc, char*const* argv, LumaController* Lumactrl)
{
	int ret = 0;

	if (NULL==Lumactrl)
	{
		ret = -1;
	}
	else if (argc == 1)
	{
		Lumactrl->Display_help();
	}
	else
	{
		int opt;
		while ((opt = getopt(argc, argv, "hi:o:a:c:s:n:")) != -1)
		{
			switch (opt)
			{
				case 'h':
				{
					Lumactrl->Display_help();
					break;
				}
				case 'i':
				{
					Lumactrl->Set_parameter(LUMA_CTRL_INPUTFILE, string(optarg));
					break;
				}
				case 'o':
				{
					Lumactrl->Set_parameter(LUMA_CTRL_OUTPUTFILE, string(optarg));
					break;
				}
				case 'a':
				{
					Lumactrl->Set_parameter(LUMA_CTRL_LUMINANCE, string(optarg));
					break;
				}
				case 'c':
				{
					Lumactrl->Set_parameter(LUMA_CTRL_COLORFORMAT, string(optarg));
					break;
				}
				case 's':
				{
					Lumactrl->Set_parameter(LUMA_CTRL_RESOLUTION, string(optarg));
					break;
				}
				case 'n':
				{
					Lumactrl->Set_parameter(LUMA_CTRL_NUMBER_OF_FRAME, string(optarg));
					break;
				}
				case '?':
				{
					ret = -1;
					break;
				}
				default :
				{
					ret = -1;
					break;
				}
			}
			if (ret != LUMA_CTRL_ERROR_NONE)
			{
				Lumactrl->Display_help();
				break;
			}
		}
	}
	return ret;
}

int main(int argc, char*const* argv)
{
	LumaController* Lumactrl = new LumaController;
	if (command_parser(argc, argv, Lumactrl) == 0)
	{
		Lumactrl->Display_parameters();
		if (Lumactrl->Adj_luminance() != LUMA_CTRL_ERROR_NONE)
		{
			printf("%s %d : Adjusting fail!!\n", __func__, __LINE__);
		}
		else
		{
			string* str = new string;
			Lumactrl->Get_parameter(LUMA_CTRL_RESOLUTION,str);
			cout << *str << endl;
			delete str;
		}
	}
	delete Lumactrl;
}
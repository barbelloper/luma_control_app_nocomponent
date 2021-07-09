/*******************************************************************************

*   Copyright (c) Telechips Inc.

*   TCC Version 1.0

This source code contains confidential information of Telechips.

Any unauthorized use without a written permission of Telechips including not
limited to re-distribution in source or binary form is strictly prohibited.

This source code is provided "AS IS" and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without
limitation, any warranty of merchantability, fitness for a particular purpose
or non-infringement of any patent, copyright or other third party intellectual
property right.
No warranty is made, express or implied, regarding the information's accuracy,
completeness, or performance.

In no event shall Telechips be liable for any claim, damages or other
liability arising from, out of or in connection with this source code or
the use in the source code.

This source code is provided subject to the terms of a Mutual Non-Disclosure
Agreement between Telechips and Company.
*
*******************************************************************************/
#include <lumacontroller.h>

void display_progress(char label[], int step, int total)
{
    const int pwidth = 72;
    int width = pwidth - strlen( label );
    int pos = ( step * width ) / total ;
    int percent = ( step * 100 ) / total;

    printf( "%s[", label );
    for ( int i = 0; i < pos; i++ )
    {
    	printf( "=");
    }
    printf( " %*c", width - pos + 1, ']' );
    printf( " %3d%%\r", percent);
}

void luma_controller_display_help()
{
	printf( "\n################################################################\n" );
	printf( "LUMA CONTROL APP!\n" );
	printf( "Option : \n" );
	printf( "\t-h : print Help page\n" );
	printf( "\t-i [inputfile]: setting inputfile path\n" );
	printf( "\t-o [outputfile]: setting outputfile path\n" );
	printf( "\t-a [number]: setting image's luminance you configure.\n" );
	printf( "\t\t\tex)./lumactrl -a 0.8\n" );
	printf( "\t-c [colorformat]: setting yuv color format\n" );
	printf( "\t\t\t\tselectable colorformat :\n" );
	printf( "\t\t\t\t\t\t\t 1. nv12\n" );
	printf( "\t\t\t\t\t\t\t 2. yuyv\n" );
	printf( "\t-s [width]x[height]: setting input image resolution\n" );
	printf( "\t\t\tex)./lumactrl -s 640x480 \n" );
	printf( "\t-n [numberofframe]: setting input image's number of frame\n" );
	printf( "###################################################################\n" );
}

LUMA_CTRL_ERROR_TYPE luma_controller_set_parameter(LUMA_CONTROLLER* lumactrl, LUMA_PARAM_INDEX paramIndex, char* willSetvalue)
{
	LUMA_CTRL_ERROR_TYPE ret = LUMA_CTRL_ERROR_NONE;

	if (NULL == lumactrl)
	{
		ret = LUMA_CTRL_NULL_POINTER_ERROR;
	}
	else
	{
		switch (paramIndex)
		{
			case LUMA_CTRL_INPUTFILE:
			{
				strcpy(lumactrl->inputfile, willSetvalue);
				break;
			}
			case LUMA_CTRL_OUTPUTFILE:
			{
				strcpy(lumactrl->outputfile, willSetvalue);
				break;
			}
			case LUMA_CTRL_LUMINANCE:
			{
				lumactrl->luminance = atof(willSetvalue);
				break;
			}
			case LUMA_CTRL_COLORFORMAT:
			{
				if (strcmp(willSetvalue, "yuyv") == 0)
				{
					lumactrl->format = YUV_422_YUYV;
				}
				else if (strcmp(willSetvalue, "nv12") == 0)
				{
					lumactrl->format = YUV_420_NV12;
				}
				else
				{
					ret = LUMA_CTRL_WRONG_PARAM;
				}
				break;
			}
			case LUMA_CTRL_RESOLUTION:
			{
				int formatLen = strlen(willSetvalue);
				int i;
				int IsResolution = 0;
				char stringwidth[10];
				char stringheight[10];

				for (i = 0; i < formatLen; i++)
				{
					if (willSetvalue[i] == 'x')
					{
						strncpy(stringwidth, willSetvalue,i);
						strncpy(stringheight, willSetvalue+i+1, formatLen - i - 1);
						IsResolution = 1;
					}
				}

				if (IsResolution)
				{
					lumactrl->width = atoi(stringwidth);
					lumactrl->height = atoi(stringheight);
				}
				else
				{
					ret = LUMA_CTRL_WRONG_PARAM;
				}
				break;
			}
			case LUMA_CTRL_NUMBER_OF_FRAME:
			{
				lumactrl->NumberOfFrame = atoi(willSetvalue);
				break;
			}
			default:
			{
				ret = LUMA_CTRL_WRONG_PARAM;
			}
		}
	}
	return ret;
}

LUMA_CTRL_ERROR_TYPE luma_controller_get_parameter(LUMA_CONTROLLER* lumactrl, LUMA_PARAM_INDEX paramIndex, char** willGetparam)
{
	LUMA_CTRL_ERROR_TYPE ret = LUMA_CTRL_ERROR_NONE;

	if (NULL == lumactrl)
	{
		ret = LUMA_CTRL_NULL_POINTER_ERROR;
	}
	else
	{
		switch (paramIndex)
		{
			case LUMA_CTRL_INPUTFILE:
			{
				strcpy(*willGetparam, lumactrl->inputfile);
				break;
			}
			case LUMA_CTRL_OUTPUTFILE:
			{
				strcpy(*willGetparam, lumactrl->outputfile);
				break;
			}
			case LUMA_CTRL_LUMINANCE:
			{
				char luma[10];
				sprintf(luma, "%f", lumactrl->luminance);
				*willGetparam = luma;
				break;
			}
			case LUMA_CTRL_COLORFORMAT:
			{
				switch (lumactrl->format)
				{
					case YUV_420_NV12:
					{
						*willGetparam = "nv12";
						break;
					}
					case YUV_422_YUYV:
					{
						*willGetparam = "yuyv";
						break;
					}
					default:
					{
						printf("%s %d : there is no parameter\n", __func__, __LINE__);
						ret = LUMA_CTRL_WRONG_PARAM;
						break;
					}
				}
				break;
			}
			case LUMA_CTRL_RESOLUTION:
			{
				char stringwidth[10];
				char stringheight[10];
				sprintf( stringwidth,"%d", lumactrl->width );
				sprintf( stringheight,"%d", lumactrl->height );
				*willGetparam = strcat(stringwidth, "x");
				*willGetparam = strcat(*willGetparam, stringheight);
				break;
			}
			case LUMA_CTRL_NUMBER_OF_FRAME:
			{
				char stringnumofframe[10];
				sprintf(stringnumofframe, "%d", lumactrl->NumberOfFrame);
				*willGetparam = stringnumofframe;
				break;
			}
			default:
			{
				printf("%s %d : there is no parameter\n", __func__, __LINE__);
				ret = LUMA_CTRL_WRONG_PARAM;
				break;
			}
		}
	}
	return ret;
}

LUMA_CONTROLLER* create_luma_controller() 
{
	return (LUMA_CONTROLLER*)malloc(sizeof(LUMA_CONTROLLER));
}

LUMA_CTRL_ERROR_TYPE destroy_luma_controller(LUMA_CONTROLLER* lumactrl)
{
	LUMA_CTRL_ERROR_TYPE ret = LUMA_CTRL_ERROR_NONE;
	if (NULL == lumactrl)
	{
		ret = LUMA_CTRL_NULL_POINTER_ERROR;
	}
	else
	{
		free(lumactrl);
	}
	return ret;
}

LUMA_CTRL_ERROR_TYPE luma_controller_command_parser(int argc, char* const* argv, LUMA_CONTROLLER* lumactrl)
{
	LUMA_CTRL_ERROR_TYPE ret = LUMA_CTRL_ERROR_NONE;

	if (NULL == lumactrl)
	{
		printf("%s,%d : PARSING ERROR!!\n", __func__, __LINE__);
		ret = LUMA_CTRL_NULL_POINTER_ERROR;
	}
	else if (argc == 1)
	{
		luma_controller_display_help();
		ret = LUMA_CTRL_WRONG_PARAM;
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
					luma_controller_display_help();
					break;
				}
				case 'i':
				{
					luma_controller_set_parameter(lumactrl, LUMA_CTRL_INPUTFILE, optarg);
					break;
				}
				case 'o':
				{
					luma_controller_set_parameter(lumactrl, LUMA_CTRL_OUTPUTFILE, optarg);
					break;
				}
				case 'a':
				{
					luma_controller_set_parameter(lumactrl, LUMA_CTRL_LUMINANCE, optarg);
					break;
				}
				case 'c':
				{
					luma_controller_set_parameter(lumactrl, LUMA_CTRL_COLORFORMAT, optarg);
					break;
				}
				case 's':
				{
 					luma_controller_set_parameter(lumactrl, LUMA_CTRL_RESOLUTION, optarg);
					break;
				}
				case 'n':
				{
					luma_controller_set_parameter(lumactrl, LUMA_CTRL_NUMBER_OF_FRAME, optarg);
					break;
				}
				case '?':
				{
					ret = LUMA_CTRL_WRONG_PARAM;
					break;
				}
				default :
				{
					ret = LUMA_CTRL_WRONG_PARAM;
					break;
				}
			}
			if (ret != LUMA_CTRL_ERROR_NONE)
			{
				luma_controller_display_help();
				break;
			}
		}
	}
	return ret;
}

void luma_controller_display_parameters(LUMA_CONTROLLER* lumactrl)
{
	printf("\n#######################IMAGE IMFORMATION#######################\n");
	printf("INPUT FILE : %s\n", lumactrl->inputfile);
	printf("OUTPUT FILE : %s\n", lumactrl->outputfile);
	printf("COLOR FORMAT : %s\n", YUV_COLOR_FORMAT_STRING[(lumactrl->format)]);
	printf("WIDTH : %d\n",lumactrl->width);
	printf("HEIGHT : %d\n", lumactrl->height);
	printf("NUMBER OF FRAME : %d\n", lumactrl->NumberOfFrame);
	printf("SETTING LUMINANCE VALUE : %0.2f\n", lumactrl->luminance);
	printf("################################################################\n");
}

LUMA_CTRL_ERROR_TYPE luma_controller_alloc_image_buffer(LUMA_CONTROLLER* lumactrl)
{
	LUMA_CTRL_ERROR_TYPE ret = LUMA_CTRL_ERROR_NONE;

	if(lumactrl == NULL)
	{
		printf("%s %d : empty lumactrl!!\n", __func__, __LINE__);
		ret = LUMA_CTRL_NULL_POINTER_ERROR;
	}
	else 
	{     
		switch (lumactrl->format)
		{
			case  YUV_420_NV12:
			{    
				int frame_size = lumactrl->width * lumactrl->height;
				lumactrl->imgBuffer = (unsigned char*)malloc( sizeof(unsigned char) * (frame_size));
				memset( lumactrl->imgBuffer, 0, sizeof(unsigned char) * frame_size );
				break;
			}
			case YUV_422_YUYV:
			{         
				int frame_size = lumactrl->width * lumactrl->height;
				lumactrl->imgBuffer = (unsigned char*)malloc( sizeof(unsigned char) * (2*frame_size));
				memset(lumactrl->imgBuffer, 0, sizeof(unsigned char) * (2*frame_size));
				break;
			}
			default : 
			{
				printf("%s %d : you choose wrong format\n", __func__, __LINE__);
				ret = LUMA_CTRL_WRONG_PARAM;
				break;
			}
		}
	}
	return ret;
}

LUMA_CTRL_ERROR_TYPE luma_controller_free_image_buffer(LUMA_CONTROLLER* lumactrl)
{
	LUMA_CTRL_ERROR_TYPE ret = LUMA_CTRL_ERROR_NONE;

	if (NULL == lumactrl)
	{
		ret = LUMA_CTRL_NULL_POINTER_ERROR;
	}
	else
	{
		free(lumactrl->imgBuffer);
		lumactrl = NULL;
	}
	return ret;
}

LUMA_CTRL_ERROR_TYPE luma_controller_adj_luminance(LUMA_CONTROLLER* lumactrlr)
{
	LUMA_CTRL_ERROR_TYPE ret = LUMA_CTRL_ERROR_NONE;
	FILE *rf = NULL;
	FILE *wf = NULL;
	rf = fopen(lumactrlr->inputfile,"rb");
	wf = fopen(lumactrlr->outputfile,"wb");

	if(rf == NULL)
	{
		ret == LUMA_CTRL_NULL_POINTER_ERROR;
	}
	else
	{
		switch (lumactrlr->format)
		{
			case YUV_420_NV12:
			{
				unsigned char* iBuffer = lumactrlr->imgBuffer;
				int nFrames = lumactrlr->NumberOfFrame;
				int framesize = lumactrlr->width*lumactrlr->height;
				int i,j;

				for(i = 0; i < nFrames; i++)
				{
					fread(iBuffer, sizeof(unsigned char), framesize, rf);
					for(j = 0; j < framesize; j++)
					{
						if(((unsigned short)iBuffer[j] * lumactrlr->luminance) > 255)
						{
							iBuffer[j] = 255;
						}
						else
						{
							iBuffer[j] = (unsigned char)(iBuffer[j] * lumactrlr->luminance);
						}
					}
					fwrite(iBuffer, sizeof(unsigned char), framesize,wf);
					fread(iBuffer, sizeof(unsigned char), framesize/2,rf);
					fwrite(iBuffer, sizeof(unsigned char), framesize/2,wf);
					memset(iBuffer, 0, sizeof(unsigned char) * framesize);
					display_progress("adjusting luma: ", i, nFrames-1);
				}
				printf("\nfinished!!\n");
				break;
			}
			case YUV_422_YUYV:
			{
				unsigned char* iBuffer = lumactrlr->imgBuffer;
				int nFrames = lumactrlr->NumberOfFrame;
				int framesize = lumactrlr->width*lumactrlr->height;
				int i,j;

				for(i = 0; i < nFrames; i++)
				{
					fread(iBuffer, sizeof(unsigned char), 2 * framesize,rf);
					for(j = 0; j < 2 * framesize; j = j + 2)
					{
						if (j > framesize * 2)
						{
							break;
						}
						if (((unsigned short)iBuffer[j] * lumactrlr->luminance) > 255)
						{
							iBuffer[j] = 255;
						}
						else
						{
							iBuffer[j] = (unsigned char)(iBuffer[j] * lumactrlr->luminance);

						}
					}
					fwrite(iBuffer, sizeof(unsigned char), (2 * framesize), wf);
					memset(iBuffer, 0, sizeof(unsigned char) * (2 * framesize));
					display_progress("adjusting luma: ",i,nFrames-1);
				}
				printf("\nfinished!!\n");
				break;
			}
			default:
			{
				printf("%s %d : wrong parameter : color format \n", __func__, __LINE__);
				ret = LUMA_CTRL_WRONG_PARAM;
				break;
			}
		}
		fclose(rf);
		fclose(wf);
	}
	return ret;
}

int main(int argc, char*const* argv)
{
	LUMA_CONTROLLER* pLumaCtrl =  create_luma_controller();
	if (luma_controller_command_parser(argc, argv, pLumaCtrl) == LUMA_CTRL_ERROR_NONE)
	{
		luma_controller_display_parameters(pLumaCtrl);
		if (luma_controller_alloc_image_buffer(pLumaCtrl) == LUMA_CTRL_ERROR_NONE)
		{
			luma_controller_adj_luminance(pLumaCtrl);
			luma_controller_free_image_buffer(pLumaCtrl);
			destroy_luma_controller(pLumaCtrl);
		}
		else
		{
			destroy_luma_controller(pLumaCtrl);
		}
	}
	else
	{
		destroy_luma_controller(pLumaCtrl);
	}
	return 0;
}

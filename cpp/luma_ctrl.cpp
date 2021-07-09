#include "luma_ctrl.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

static void Display_progress(string label, int step, int total)
{
    const int pwidth = 72;
    int width = pwidth - label.length();
    int pos = ( step * width ) / total ;
    int percent = ( step * 100 ) / total;

    printf( "%s[", label.c_str() );
    for ( int i = 0; i < pos; i++ )
    {
    	printf( "=");
    }
    printf( " %*c", width - pos + 1, ']' );
    printf( " %3d%%\r", percent);
}

LumaController::LumaController()
{
	mWidth = 0;
	mHeight = 0;
	mNumberOfFrame = 0;
	mLuminance = 1.0;
	mImgBuffer = NULL;
}

LumaController::~LumaController()
{
	Free_image_buffer();
}

/** Public functions */
LUMA_CTRL_ERROR_TYPE LumaController::Set_parameter(LUMA_PARAM_INDEX paramIndex, string willSetparam)
{
	LUMA_CTRL_ERROR_TYPE ret = LUMA_CTRL_ERROR_NONE;

	switch (paramIndex)
	{
		case LUMA_CTRL_INPUTFILE:
		{
			mInputfile = willSetparam;
			break;
		}
		case LUMA_CTRL_OUTPUTFILE:
		{
			mOutputfile = willSetparam;
			break;
		}
		case LUMA_CTRL_LUMINANCE:
		{
			mLuminance = stof(willSetparam);
			break;
		}
		case LUMA_CTRL_COLORFORMAT:
		{
			if ("yuyv" == willSetparam)
			{
				mFormat = YUV_422_YUYV;
			}
			else if ("nv12" == willSetparam)
			{
				mFormat = YUV_420_NV12;
			}
			else
			{
				ret = LUMA_CTRL_WRONG_PARAM;
			}
			break;
		}
		case LUMA_CTRL_RESOLUTION:
		{
			int formatLen = willSetparam.length();
			int i;
			int IsResolution = 0;
			string width;
			string height;

			for (i = 0; i < formatLen; i++)
			{
				if (willSetparam[i] == 'x')
				{
					width = willSetparam.substr(0,i);
					height = willSetparam.substr(i+1, formatLen - i - 1);
					IsResolution = 1;
				}
			}

			if (IsResolution)
			{
				mWidth = stoi(width);
				mHeight = stoi(height);
			}
			else
			{
				ret = LUMA_CTRL_WRONG_PARAM;
			}
			break;
		}
		case LUMA_CTRL_NUMBER_OF_FRAME:
		{
			mNumberOfFrame = stoi(willSetparam);
			break;
		}
		default:
		{
			ret = LUMA_CTRL_WRONG_PARAM;
		}
	}
	return ret;
}

LUMA_CTRL_ERROR_TYPE LumaController::Get_parameter(LUMA_PARAM_INDEX paramIndex, string* willGetparam)
{
	LUMA_CTRL_ERROR_TYPE ret = LUMA_CTRL_ERROR_NONE;

	switch (paramIndex)
	{
		case LUMA_CTRL_INPUTFILE:
		{
			*willGetparam = string(mInputfile);
			break;
		}
		case LUMA_CTRL_OUTPUTFILE:
		{
			*willGetparam = string(mOutputfile);
			break;
		}
		case LUMA_CTRL_LUMINANCE:
		{
			char luma[10];
			sprintf(luma, "%f", mLuminance);
			*willGetparam = string(luma);
			break;
		}
		case LUMA_CTRL_COLORFORMAT:
		{
			if (mFormat == YUV_422_YUYV)
			{
				*willGetparam = "yuyv";
			}
			else if (mFormat == YUV_420_NV12)
			{
				*willGetparam = "nv12";
			}
			else
			{
				ret = LUMA_CTRL_WRONG_PARAM;
			}
			break;
		}
		case LUMA_CTRL_RESOLUTION:
		{
			char stringwidth[10];
			char stringheight[10];
			sprintf(stringwidth,"%d", mWidth);
			sprintf(stringheight,"%d", mHeight);
			*willGetparam = string(stringwidth) + "x" + string(stringheight);
			break;
		}
		case LUMA_CTRL_NUMBER_OF_FRAME:
		{
			char stringnumofframe[10];
			*willGetparam = sprintf(stringnumofframe, "%d", mNumberOfFrame);
			break;
		}
		default:
		{
			ret = LUMA_CTRL_WRONG_PARAM;
		}
	}
	return ret;
}

LUMA_CTRL_ERROR_TYPE LumaController::Adj_luminance(void)
{
	LUMA_CTRL_ERROR_TYPE ret = Alloc_image_buffer();
	FILE *rf = NULL;
	FILE *wf = NULL;
	rf = fopen(mInputfile.c_str(),"rb");
	wf = fopen(mOutputfile.c_str(),"wb");

	if (LUMA_CTRL_ERROR_NONE != ret)
	{
		printf("%s %d : error : image buffer allocate\n",__func__,__LINE__);
	}
	else if(NULL == rf )
	{
		printf("%s %d : error : file read\n" ,__func__, __LINE__);
		ret == LUMA_CTRL_NULL_POINTER_ERROR;
	}
	else
	{
		switch (mFormat)
		{
			case YUV_420_NV12:
			{
				unsigned char* iBuffer = mImgBuffer;
				int nFrames = mNumberOfFrame;
				int framesize = mWidth * mHeight;
				int i,j;

				for(i = 0; i < nFrames; i++)
				{
					fread(iBuffer, sizeof(unsigned char), framesize, rf);
					for(j = 0; j < framesize; j++)
					{
						if(((unsigned short)iBuffer[j] * mLuminance) > 255)
						{
							iBuffer[j] = 255;
						}
						else
						{
							iBuffer[j] = (unsigned char)(iBuffer[j] * mLuminance);
						}
					}
					fwrite(iBuffer, sizeof(unsigned char), framesize,wf);
					fread(iBuffer, sizeof(unsigned char), framesize/2,rf);
					fwrite(iBuffer, sizeof(unsigned char), framesize/2,wf);
					memset(iBuffer, 0, sizeof(unsigned char) * framesize);
					Display_progress("adjusting luma: ", i, nFrames-1);
				}
				printf("\nfinished!!\n");
				break;
			}
			case YUV_422_YUYV:
			{
				unsigned char* iBuffer = mImgBuffer;
				int nFrames = mNumberOfFrame;
				int framesize = mWidth * mHeight;
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
						if (((unsigned short)iBuffer[j] * mLuminance) > 255)
						{
							iBuffer[j] = 255;
						}
						else
						{
							iBuffer[j] = (unsigned char)(iBuffer[j] * mLuminance);
						}
					}
					fwrite(iBuffer, sizeof(unsigned char), (2 * framesize), wf);
					memset(iBuffer, 0, sizeof(unsigned char) * (2 * framesize));
					Display_progress("adjusting luma: ",i,nFrames-1);
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

void LumaController::Display_parameters(void)
{
	printf("\n#######################IMAGE IMFORMATION#######################\n");
	printf("INPUT FILE : %s\n", mInputfile.c_str());
	printf("OUTPUT FILE : %s\n", mOutputfile.c_str());
	if (mFormat == YUV_420_NV12)
	{
		printf("COLOR FORMAT : YUV_420_NV12\n");
	}
	else if (mFormat == YUV_422_YUYV)
	{
		printf("COLOR FORMAT : YUV_422_YUYV\n");
	}
	else
	{
		printf("COLOR FORMAT : Wrong color format\n");
	}
	printf("WIDTH : %d\n",mWidth);
	printf("HEIGHT : %d\n", mHeight);
	printf("NUMBER OF FRAME : %d\n", mNumberOfFrame);
	printf("SETTING LUMINANCE VALUE : %0.2f\n", mLuminance);
	printf("################################################################\n");
}

void LumaController::Display_help(void)
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

LUMA_CTRL_ERROR_TYPE LumaController::Alloc_image_buffer(void)
{
	LUMA_CTRL_ERROR_TYPE ret = LUMA_CTRL_ERROR_NONE;

	switch (mFormat)
	{
		case  YUV_420_NV12:
		{
			int frame_size = mWidth * mHeight;
			mImgBuffer = new unsigned char[frame_size] {0,};
			break;
		}
		case YUV_422_YUYV:
		{
			int frame_size = mWidth * mHeight;
			mImgBuffer = new unsigned char[2*frame_size] {0,};
			break;
		}
		default :
		{
			printf("%s %d : you choose wrong format\n", __func__, __LINE__);
			ret = LUMA_CTRL_WRONG_PARAM;
			break;
		}
	}
	return ret;
}

LUMA_CTRL_ERROR_TYPE LumaController::Free_image_buffer(void)
{
	delete mImgBuffer;
}


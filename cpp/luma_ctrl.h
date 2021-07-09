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

/**
 * This enum type define error type for lumacontroller.
 */
#ifndef _LUMA_CONTROLLER_H_
#define _LUMA_CONTROLLER_H_
#include <string>

using namespace std;


typedef enum LUMA_CTRL_ERROR_TYPE
{
	LUMA_CTRL_PARSING_ERROR = -1,		/**< parsing error */
	LUMA_CTRL_ERROR_NONE,				/**< none error, all situation normal */
	LUMA_CTRL_NULL_POINTER_ERROR,		/**< null pointer input error */
	LUMA_CTRL_WRONG_PARAM				/**< worng parameter input error */
} LUMA_CTRL_ERROR_TYPE;

/**
 * The COLOR_FORMAT_TYPE enumeration is used to indicate YUV color format.
 */
typedef enum YUV_COLOR_FORMAT
{
	WRONG_FORMAT=-1,
	YUV_420_NV12,		/**< yuv420 nv12 planner format */
	YUV_422_YUYV		/**< yuv422 yuyv packed format */
} YUV_COLOR_FORMAT;

/**
 * This type defines the parameters index to get or set.
 */
typedef enum LUMA_PARAM_INDEX
{
	LUMA_CTRL_INPUTFILE,			/**< Input file path */
	LUMA_CTRL_OUTPUTFILE,			/**< Output file path */
	LUMA_CTRL_LUMINANCE,			/**< Image luminance */
	LUMA_CTRL_COLORFORMAT,			/**< Image Color format */
	LUMA_CTRL_RESOLUTION,			/**< Image resolution */
	LUMA_CTRL_NUMBER_OF_FRAME		/**< a number of Image frame */
} LUMA_PARAM_INDEX;

/**
 * This array is defined to display YUV_COLOR_FORMAT type's string
 */


class LumaController
{
public:
	LumaController();
	virtual ~LumaController();

	LUMA_CTRL_ERROR_TYPE Set_parameter(LUMA_PARAM_INDEX paramIndex, string willSetparam);

	LUMA_CTRL_ERROR_TYPE Adj_luminance(void);

	LUMA_CTRL_ERROR_TYPE Get_parameter(LUMA_PARAM_INDEX paramIndex, string* willGetparam);

	void Display_parameters(void);

	void Display_help(void);

private:
	string mInputfile;             /**< Input file path */
	string mOutputfile;            /**< Output file path */

	YUV_COLOR_FORMAT mFormat;        /**< Image color format */
	int mWidth;                      /**< Image resolution : width */
	int mHeight;                     /**< Image resolution : height */

	int mNumberOfFrame;              /**< a Number of Image frame */

	float mLuminance;                /**< Image luminance */
	unsigned char* mImgBuffer;       /**< Image buffer */

	LUMA_CTRL_ERROR_TYPE Alloc_image_buffer(void);

	LUMA_CTRL_ERROR_TYPE Free_image_buffer(void);
};

#endif

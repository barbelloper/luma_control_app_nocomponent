/**
 * \file lumacontroll.h
 * \brief Common header file for lumacontroller.
 */
#ifndef _LUMA_CONTROLLER_H_
#define _LUMA_CONTROLLER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * This enum type define error type for lumacontroller.
 */
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
 * LUMA_CONTROLLER Struct is used to indicate input image instance/context.
 */
typedef struct LUMA_CONTROLLER
{
	char inputfile[50];				/**< Input file path */
	char outputfile[50];			/**< Output file path */
	YUV_COLOR_FORMAT format;		/**< Image color format */
	int width;						/**< Image resolution : width */
	int height;						/**< Image resolution : height */
	int NumberOfFrame;				/**< a Number of Image frame */
	float luminance;				/**< Image luminance */
	unsigned char* imgBuffer;		/**< Image buffer */
} LUMA_CONTROLLER;

/**
 * This array is defined to display YUV_COLOR_FORMAT type's string
 */
char* YUV_COLOR_FORMAT_STRING[] = 
{
	"YUV_420_NV12", 
	"YUV_422_YUYV"
};

/**
 * This function is defined to display luminance adjusting progress by percentage
 */
void display_progress(char label[], int step, int total);

/**
 * This funtion create image instance(LUMA_CONTROLLER)
 * \retval LUMA_CONTROLLER* type
 */
LUMA_CONTROLLER* create_luma_controller();

/**
 * This funtion destroy image instance(LUMA_CONTROLLER)
 * \param[in] lumactrl
 *			 reference to the image instance returned by create_luma_controller()
 * \retval LUMA_CTRL_NULL_POINTER_ERROR input null instance.
 * \retval LUMA_CTRL_ERROR_NONE success to destroy instance.
 */
LUMA_CTRL_ERROR_TYPE destroy_luma_controller(LUMA_CONTROLLER * lumactrl);

/**
 * This funtion parse user command to image instance(LUMA_CONTROLLER)
 * \param[in] argc
 *			 a number of main function's argument
 * \param[in] arv
 * 			 array of main function's arguments
 * \param[in,out] lumactrl
 * 				 in : empty image instance
 * 				 out : set image instance by user command
 *
 * \retval LUMA_CTRL_PARSING_ERROR parsing error.
 * \retval LUMA_CTRL_WRONG_PARAM unknown command. call luma_controller_display_help().
 * \retval LUMA_CTRL_ERROR NONE success to parse user command.
 */
LUMA_CTRL_ERROR_TYPE luma_controller_command_parser(int argc, char* const* argv, LUMA_CONTROLLER* lumactrl);

/**
 * This funtion display information of image instance(LUMA_CONTROLLER).
 * \param[in] lumactrl
 * 			 image instance to display information.
 *
 * \retval LUMA_CTRL_NULL_POINTER_ERROR input null instance.
 */
void luma_controller_display_parameters(LUMA_CONTROLLER* lumactrl);

/**
 * This funtion allocation image buffer.
 * \param[in] lumactrl
 * 			 image instance to allocation buffer.
 *
 * \retval LUMA_CTRL_NULL_POINTER_ERROR input null instance.
 * \retval LUMA_CTRL_ERROR_NONE success to allocate buffer.
 * \retval LUMA_CTRL_WRONG_PARAM input wrong parameter.
 */
LUMA_CTRL_ERROR_TYPE luma_controller_alloc_image_buffer(LUMA_CONTROLLER* lumactrl);

/**
 * This funtion free image buffer.
 * \param[in] lumactrl
 * 			 image instance to free buffer.
 * \retval LUMA_CTRL_NULL_POINTER_ERROR input null instance.
 * \retval LUMA_CTRL_ERROR_NONE success to free image buffer.
 */
LUMA_CTRL_ERROR_TYPE luma_controller_free_image_buffer(LUMA_CONTROLLER* lumactrl);

/**
 * This funtion adjust image luminance that was set by user.
 *\param[in] lumactrl
 * 			 image instance to adjust luminance.
 *
 * \retval LUMA_CTRL_PARSING_ERROR allocation buffer error.
 * \retval LUMA_CTRL_WRONG_PARAM input wrong parameter.
 * \retval LUMA_CTRL_ERROR_NONE complete adjusting luminance.
 */
LUMA_CTRL_ERROR_TYPE luma_controller_adj_luminance(LUMA_CONTROLLER* lumactrl);

/**
 * This funtion set image parameter by selected index in LUMA_PARAM_INDEX type.
 * It usually called by luma_controller_command_parser().
 * \param[in,out] lumactrl
 * 			 	 in : image instance to set parameter.
 * 				 out : set image instance by selected index.
 * \param[in] paramIndex
 * 			 input parameter Index that want to set.
 * \param[in] willSetparam
 * 			 string type of parameter that will be set. (command)
 * 			 like "yuyv","640x480"
 *
 * \retval LUMA_CTRL_NULL_POINTER_ERROR input null
 * \retval LUMA_CTRL_WRONG_PARAM input wrong parameter.
 * \retval LUMA_CTRL_ERROR_NONE success to set parameter.
 */
LUMA_CTRL_ERROR_TYPE luma_controller_set_parameter(LUMA_CONTROLLER* lumactrl, LUMA_PARAM_INDEX paramIndex, char* willSetparam);

/**
 * This funtion get image parameter by selected index in LUMA_PARAM_INDEX type.
 * \param[in] lumactrl
 * 			 	 in : image instance to set parameter.
 * 				 out : set image instance by selected index.
 * \param[in] paramIndex
 * 			 input parameter Index that want to get.
 * \param[in,out] willSetparam
 * 			 int : reference to string type of parameter that will be got. (command).
 * 			 out : output reference to string type of parameter.
 *
 * \retval LUMA_CTRL_NULL_POINTER_ERROR input null
 * \retval LUMA_CTRL_WRONG_PARAM input wrong parameter.
 * \retval LUMA_CTRL_ERROR_NONE success to get parameter.
 */
LUMA_CTRL_ERROR_TYPE luma_controller_get_parameter(LUMA_CONTROLLER* lumactrl, LUMA_PARAM_INDEX paramIndex, char** willGetparam);

/**
 *This function display guide for this application.
 */
void luma_controller_display_help();

#endif
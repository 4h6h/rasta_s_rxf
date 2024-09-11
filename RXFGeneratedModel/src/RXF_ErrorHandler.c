/*********************************************************************
	Rhapsody in C	: 9.0.2 
	Login		: devuser
	Component	: RXF_Sandbox 
	Configuration 	: VisualStudio
	Model Element	: RXF_ErrorHandler
	Generated Date	: Tue, 13, Aug 2024  
	File Path	: ../Generated/VisualStudio/RXF_ErrorHandler.c
*********************************************************************/

#include "RXF_ErrorHandler.h"
#include "log.h"
/*## package Sandbox::rxferrorhandler */

/*## class TopLevel::RXF_ErrorHandler */
/*## operation RXF_ErrorHandler_error(const ErrorCode,const int_t,const bool) */
void RXF_ErrorHandler_error(const ErrorCode aErrorCode, const int_t additionalArgument, const bool returnAllowed) {
    /*#[ operation RXF_ErrorHandler_error(const ErrorCode,const int_t,const bool) */
    (void)additionalArgument;
    (void)aErrorCode;
    /* 
    Called by the RXF if any error is detected. This operation is by default implemented as an endless loop, blocking the framework.
    It is strongly recommended to provide your own error handler implementation, which fits your specific application and environment
    needs. This can be done by aplying the Stereotype  <<RXFConfigurationPackage>> to a new Package in your user Model.
    The Default Error Handler will be overridden.
    Returning from this function also is a possibility, depending on the impact of a specific error code and the application.
    */
    
    /* If error handler is called with argument returnAllowed false, a return would have major implications.
       Do not delete this while loop. Place your code to safely reboot or shut down your system here. */
    while(!returnAllowed)
    {
    
    }
    LOG_DEBUG("Error Handler %u", aErrorCode);
    /*#]*/
}

/*********************************************************************
	File Path	: ../Generated/VisualStudio/RXF_ErrorHandler.c
*********************************************************************/

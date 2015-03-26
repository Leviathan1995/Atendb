#include "Error.h"
Error::Error()
{
	Error_Code = NO_Error;
	Function = "";
	Module = "";
	Error_string = "";
}
Error::Error(int error_Code, string module, string function, string error_string)
{
	Error_Code = error_Code;
	Function = function;
	Module = module;
	Error_string = error_string;
}
#pragma once

#include <stdexcept>

class ImageException : public std::exception {
public:
    enum class ErrCode {
        NoFileName,
        NoFilters,
        NoBMP,
        NoSuchFilter,
        WrongBlurParams,
        WrongCropParams,
        WrongNegParams,
        WrongGsParams,
        WrongSharpParams,
        WrongEdgeParams,
        WrongBlocksParams,
        OutputProblem
    };

    ImageException(ErrCode code) : err_code_(code) {};
    ErrCode GetExceptionType() {return err_code_;}
protected:
    ErrCode err_code_;
};

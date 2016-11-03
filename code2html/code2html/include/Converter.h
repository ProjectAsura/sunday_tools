//-------------------------------------------------------------------------------------------------
// File : Converter.h
// Desc : Converter Module.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once


//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <cstdio>
#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////////////////////////
// LANGUAGE_TYPES enum
///////////////////////////////////////////////////////////////////////////////////////////////////
enum LANGUAGE_TYPES
{
    LANGUAGE_TYPE_CPP,      //!< C/C++言語.
    LANGUAGE_TYPE_HLSL,     //!< DirectX High Level Shader Language.
    LANGUAGE_TYPE_GLSL,     //!< OpenGL Shader Language.
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// Converter
///////////////////////////////////////////////////////////////////////////////////////////////////
class Converter
{
    //=============================================================================================
    // list of friend classes and methods.
    //=============================================================================================
    /* NOTHING */

public:
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Config structure
    ///////////////////////////////////////////////////////////////////////////////////////////////
    struct Config
    {
        std::string     InputFile;          //!< 入力ファイル名.
        std::string     OutputFile;         //!< 出力ファイル名.
        std::string     RowColor;           //!< 行番号カラー.
        std::string     StringColor;        //!< 文字列カラー.
        std::string     CommentColor;       //!< コメントカラー.
        std::string     KeywordColor;       //!< キーワードカラー.
        LANGUAGE_TYPES  Type;               //!< 言語タイプ.

        //-----------------------------------------------------------------------------------------
        //! @brief      コンストラクタです.
        //-----------------------------------------------------------------------------------------
        Config()
        : RowColor      ( "#2b91af" )
        , StringColor   ( "#a31515" )
        , CommentColor  ( "#008000" )
        , KeywordColor  ( "blue" )
        , Type          ( LANGUAGE_TYPE_CPP )
        { /* DO_NOTHING */ }
    };

    //=============================================================================================
    // public variables.
    //=============================================================================================
    /* NOTHING */

    //=============================================================================================
    // public methods.
    //=============================================================================================

    //---------------------------------------------------------------------------------------------
    //! @brief      変換処理を行います.
    //!
    //! @param[in]      config      設定情報.
    //! @return     変換処理に成功したら true を返却します.
    //---------------------------------------------------------------------------------------------
    static bool Process(const Config& config);

private:
    //=============================================================================================
    // private variables.
    //=============================================================================================
    /* NOTHING */

    //=============================================================================================
    // private methods.
    //=============================================================================================

    //---------------------------------------------------------------------------------------------
    //! @brief      有効な字であるかどうかチェックします.
    //---------------------------------------------------------------------------------------------
    static bool IsNotWord(const char c);

    //---------------------------------------------------------------------------------------------
    //! @brief      ヘッダを書き込みます.
    //---------------------------------------------------------------------------------------------
    static void WriteHeader(FILE* pFile, LANGUAGE_TYPES type);

    //---------------------------------------------------------------------------------------------
    //! @brief      フッタを書き込みます.
    //---------------------------------------------------------------------------------------------
    static void WriteFooter(FILE* pFile);

    //---------------------------------------------------------------------------------------------
    //! @brief      キーワードを読み込みます.
    //---------------------------------------------------------------------------------------------
    static void LoadKeyword(LANGUAGE_TYPES type, std::vector<std::string>& list);

    //---------------------------------------------------------------------------------------------
    //! @brief      プリプロセッサを読み込みます.
    //---------------------------------------------------------------------------------------------
    static void LoadPreprocessor(LANGUAGE_TYPES type, std::vector<std::string>& list);
};


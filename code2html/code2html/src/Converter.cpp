//-------------------------------------------------------------------------------------------------
// File : Converter.cpp
// Desc : Converter Module.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include "Converter.h"
#include <mbstring.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Converter class
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//      有効な字であるかどうかチェックします.
//-------------------------------------------------------------------------------------------------
bool Converter::IsNotWord(const char value)
{
    if ( value >= 'a' && value <= 'z' )
    { return false; }

    if ( value >= 'A' && value <= 'Z' )
    { return false; }

    if ( value >= '0' && value <= '9' )
    { return false; }

    if ( value == '_' )
    { return false; }

    return true;
}

//-------------------------------------------------------------------------------------------------
//      ヘッダを書き込みます.
//-------------------------------------------------------------------------------------------------
void Converter::WriteHeader(FILE* pFile, LANGUAGE_TYPES type)
{
    fprintf_s( pFile, "<html>\n" );
    fprintf_s( pFile, "<head>\n" );
    fprintf_s( pFile, "<title>Create Byte Code2Html</title>\n");

    if ( type == LANGUAGE_TYPE_CPP )
    { fprintf_s( pFile, "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n" ); }
    else
    { fprintf_s( pFile, "<meta http-equiv=\"content-type\" content=\"text/html; charset=shift_jis\">\n" ); }
    fprintf_s( pFile, "<meta http-equiv=\"content-style-type\" content=\"text/css\">\n" );
    fprintf_s( pFile, "<style type=\"text/css\">\n" );
    fprintf_s( pFile, "pre {\n");
    fprintf_s( pFile, "    font-size:10.0pt;\n" );
    fprintf_s( pFile, "    font-family:\"Courier New\";\n" );
    fprintf_s( pFile, "}\n" );
    fprintf_s( pFile, "</style>\n" );
    fprintf_s( pFile, "<body>\n" );
    fprintf_s( pFile, "<pre>\n" );
}

//-------------------------------------------------------------------------------------------------
//      フッタを書き込みます.
//-------------------------------------------------------------------------------------------------
void Converter::WriteFooter(FILE* pFile)
{
    fprintf_s( pFile, "</pre>\n" );
    fprintf_s( pFile, "</body>\n" );
    fprintf_s( pFile, "</html>\n" );
}

//-------------------------------------------------------------------------------------------------
//      キーワードを読み込みます.
//-------------------------------------------------------------------------------------------------
void Converter::LoadKeyword(LANGUAGE_TYPES type, std::vector<std::string>& list)
{
    switch( type )
    {
    case LANGUAGE_TYPE_CPP:
        {
            std::string keywords[] = {
                #include "KeywordsCpp.h"
            };
            auto size = sizeof(keywords) / sizeof(keywords[0]);
            list.resize( size );

            for( unsigned int i=0; i<size; ++i )
            { list[i] = keywords[i]; }

            list.shrink_to_fit();
        }
        break;

    case LANGUAGE_TYPE_GLSL:
        {
            std::string keywords[] = {
                #include "KeywordsGlsl.h"
            };
            auto size = sizeof(keywords) / sizeof(keywords[0]);
            list.resize( size );

            for( unsigned int i=0; i<size; ++i )
            { list[i] = keywords[i]; }

            list.shrink_to_fit();
        }
        break;

    case LANGUAGE_TYPE_HLSL:
        {
            std::string keywords[] = {
                #include "KeywordsHlsl.h"
            };
            auto size = sizeof(keywords) / sizeof(keywords[0]);
            list.resize( size );

            for( unsigned int i=0; i<size; ++i )
            { list[i] = keywords[i]; }

            list.shrink_to_fit();
        }
        break;
    }
}

//-------------------------------------------------------------------------------------------------
//      プリプロセッサを読み込みます.
//-------------------------------------------------------------------------------------------------
void Converter::LoadPreprocessor(LANGUAGE_TYPES type, std::vector<std::string>& list)
{
    switch( type )
    {
    case LANGUAGE_TYPE_CPP:
        {
            std::string preprocessor[] = {
                #include "PreprocessorCpp.h"
            };
            auto size = sizeof(preprocessor) / sizeof(preprocessor[0]);
            list.resize( size );

            for( unsigned int i=0; i<size; ++i )
            { list[i] = preprocessor[i]; }

            list.shrink_to_fit();
        }
        break;

    case LANGUAGE_TYPE_GLSL:
        {
            std::string preprocessor[] = {
                #include "PreprocessorGlsl.h"
            };
            auto size = sizeof(preprocessor) / sizeof(preprocessor[0]);
            list.resize( size );

            for( unsigned int i=0; i<size; ++i )
            { list[i] = preprocessor[i]; }

            list.shrink_to_fit();
        }
        break;

    case LANGUAGE_TYPE_HLSL:
        {
            std::string preprocessor[] = {
                #include "PreprocessorHlsl.h"
            };
            auto size = sizeof(preprocessor) / sizeof(preprocessor[0]);
            list.resize( size );

            for( unsigned int i=0; i<size; ++i )
            { list[i] = preprocessor[i]; }

            list.shrink_to_fit();
        }
        break;
    }
}

//-------------------------------------------------------------------------------------------------
//      変換処理を行います.
//-------------------------------------------------------------------------------------------------
bool Converter::Process(const Config& config)
{
    // 引数をチェック.
    if ( config.InputFile.empty() )
    {
        printf_s( "Error : 入力ファイルが設定されていません.\n" );
        return false;
    }

    if ( config.OutputFile.empty() )
    {
        printf_s( "Error : 出力ファイルが設定されていません.\n" );
        return false;
    }

    // 入力ファイルを開く.
    FILE* pFile;
    auto err = fopen_s( &pFile, config.InputFile.c_str(), "r" );
    if ( err != 0 )
    {
        printf_s( "Error : 入力ファイルが開けませんでした.\n" );
        return false;
    }

    fpos_t start, end;
    auto ret = fgetpos( pFile, &start );
    if ( ret != 0 )
    {
        printf_s( "Error : ファイルサイズの算出に失敗しました. ErrorCode = 1\n" );
        fclose( pFile );
        return false;
    }
    fseek( pFile, 0, SEEK_END );
    ret = fgetpos( pFile, &end );
    if ( ret != 0 )
    {
        printf_s( "Error : ファイルサイズの算出に失敗しました. ErrorCode = 2\n" );
        fclose( pFile );
        return false;
    }
    fseek( pFile, 0, SEEK_SET );

    if ( start == -1 || end == -1 )
    {
        printf_s( "Error : ファイルサイズの算出に失敗しました. ErrorCode = 3\n" );
        fclose( pFile );
        return false;
    }

    // ファイルサイズを算出する.
    auto size = static_cast<int>(end - start);

    // バッファのメモリを確保.
    auto pBuffer = malloc( size + 1 );
    if ( pBuffer == nullptr )
    {
        printf_s( "Error : メモリ確保に失敗しました.\n" );
        fclose( pFile );
        return false;
    }

    // NULL終端化.
    memset( pBuffer, '\0', size + 1 );

    // 一括読み込み.
    fread( pBuffer, size, 1, pFile );

    // 入力ファイルを閉じる.
    fclose( pFile );

    // 出力ファイルを開く.
    err = fopen_s( &pFile, config.OutputFile.c_str(), "w" );
    if ( err != 0 )
    {
        printf_s( "Error : 出力ファイルが開けませんでした.\n" );
        free(pBuffer);
        fclose( pFile );
        return false;
    }

    auto row = 0;
    auto isEsc = false;
    auto isNewLine = true;
    auto isPreprocessor = false;
    auto commentCount = 0;
    auto singleCommentCount = 0;
    auto stringCount = 0;
    auto charCount = 0;

    // 言語別のキーワード，プリプロセッサの単語を読み込み.
    std::vector<std::string> keywordList;
    std::vector<std::string> preprocessorList;
    LoadKeyword( config.Type, keywordList );
    LoadPreprocessor( config.Type, preprocessorList );

    // 先頭ポインタを設定.
    auto buffer = static_cast<char*>(pBuffer);

    // ヘッダ書き込み.
    WriteHeader( pFile, config.Type );

    // ソースコードを変換.
    while( true )
    {
        auto offset = buffer - static_cast<char*>( pBuffer );

    #if 0
        printf_s( "%c", buffer[0] );
    #endif

        if ( buffer[0] == '\0' || offset >= size )
        { break; }

        if ( isNewLine )
        {
            isNewLine = false;
            isPreprocessor = false;
            singleCommentCount = 0;

            row++;

            fprintf_s( pFile, "<font color=\'%s\'>%05d:</font>  ", config.RowColor.c_str(), row );
            isEsc = false;
        }
        else
        {
            isEsc = ( buffer[-1] == '\\' && buffer[-2] == '\\' && ( buffer[-3] != '\'' ) );
        }

        if ( stringCount == 0 && charCount == 0 )
        {
            // Cスタイルコメント 開始.
            if ( buffer[0] == '/' && buffer[1] == '*' )
            {
                if ( commentCount == 0 && singleCommentCount == 0 )
                { fprintf_s( pFile, "<font color=\'%s\'>/*", config.CommentColor.c_str() ); }
                commentCount++;
                buffer += 2;
                continue;;
            }
            // Cスタイルコメント 終了.
            else if ( buffer[0] == '*' && buffer[1] == '/' )
            {
                commentCount--;
                if ( commentCount == 0 && singleCommentCount == 0 )
                { fprintf_s( pFile, "/*</font>" ); }
                else
                { fprintf_s( pFile, "/*" ); }
                buffer += 2;
                continue;
            }

            // C++スタイルコメント
            if ( buffer[0] == '/' && buffer[1] == '/' )
            {
                if ( commentCount == 0 && singleCommentCount == 0 )
                { fprintf_s( pFile, "<font color=\'%s\'>//", config.CommentColor.c_str() ); }
                else
                { fprintf_s( pFile, "//" ); }
                singleCommentCount++;
                buffer += 2;
                continue;
            }

            // XMLスタイルコメント.
            if ( buffer[0] == '/' && buffer[1] == '/' && buffer[2]  == '/' )
            {
                if ( commentCount == 0 && singleCommentCount == 0 )
                { fprintf_s( pFile, "<font color=\'%s\'>///", config.CommentColor.c_str() ); }
                else
                { fprintf_s( pFile, "///"); }
                buffer += 3;
                singleCommentCount++;
                continue;
            }

            if ( buffer[0] == '\r' || buffer[0] ==  '\n' )
            {
                if ( singleCommentCount > 0 && commentCount == 0 )
                { fprintf_s( pFile, "</font>" ); }
            }
        }

        if ( commentCount == 0 && singleCommentCount == 0 )
        {
            // 文字列.
            if ( buffer[0] == '\"' && isEsc == false )
            {
                if ( stringCount == 0 && charCount == 0 )
                {
                    fprintf_s( pFile, "<font color=\'%s\'>&quot;", config.StringColor.c_str() );
                    buffer++;
                    stringCount++;
                    continue;
                }
                else if ( stringCount > 0 && charCount == 0 )
                {
                    fprintf_s( pFile, "&quot;</font>" );
                    buffer++;
                    stringCount--;
                    continue;
                }
            }

            // 文字.
            if ( buffer[0] == '\'' && isEsc == false )
            {
                if ( stringCount == 0 && charCount == 0 )
                {
                    fprintf_s( pFile, "<font color=\'%s\'>\'", config.StringColor.c_str() );
                    buffer++;
                    charCount++;
                    continue;
                }
                else if ( stringCount == 0 && charCount > 0 )
                {
                    fprintf_s( pFile, "\'</font>" );
                    buffer++;
                    charCount--;
                    continue;
                }
            }
        }

        if ( commentCount == 0 
          && singleCommentCount == 0
          && charCount == 0
          && stringCount == 0 )
        {
            // ##__VA_ARGS__のようなものをはじく.
            if ( buffer[0] == '#' && buffer[1] == '#' )
            {
                fprintf_s( pFile, "##" );
                buffer += 2;
                continue;
            }

            // #define などのプリプロセッサ.
            if ( buffer[0] == '#' && buffer[1] != '#' )
            {
                fprintf_s( pFile, "<font color=\'%s\'>#</font>", config.KeywordColor.c_str() );
                buffer++;
                isPreprocessor = true;
                continue;
            }
            else
            {
                // キーワードを処理.
                auto prev = true;
                auto next = true;
                auto find = false;
                if ( isNewLine == false )
                { prev = IsNotWord( buffer[-1] ); }

                for( auto keyword : keywordList )
                {
                    if ( keyword.empty() )
                    { break; }

                    next = IsNotWord( buffer[keyword.size()] );

                    if ( !prev || !next )
                    { continue; }

                    if ( _mbsnbcmp( 
                            reinterpret_cast<const unsigned char*>(buffer),
                            reinterpret_cast<const unsigned char*>(keyword.c_str()),
                            keyword.size() ) == 0 )
                    {
                        fprintf_s( pFile, "<font color=\'%s\'>%s</font>", config.KeywordColor.c_str(), keyword.c_str() );
                        buffer += keyword.size();
                        find = true;
                        break;
                    }
                }

                // 見つかったら次へ.
                if (find)
                { continue; }

                // #で始まっている場合は define, includeなどの単語が該当するかチェック.
                if ( isPreprocessor )
                {
                    prev = true;
                    next = true;
                    if ( isNewLine == false )
                    { prev = IsNotWord( buffer[-1] ); }

                    find = false;

                    for( auto preprocessor : preprocessorList )
                    {
                        if ( preprocessor.empty() )
                        { break; }

                        next = IsNotWord( buffer[preprocessor.size()] );

                        if ( !prev || !next )
                        { continue; }

                        if ( _mbsnbcmp( 
                                reinterpret_cast<const unsigned char*>(buffer),
                                reinterpret_cast<const unsigned char*>(preprocessor.c_str()),
                                preprocessor.size()) == 0 )
                        {
                            fprintf_s( pFile, "<font color=\'%s\'>%s</font>", config.KeywordColor.c_str(), preprocessor.c_str() );
                            buffer += preprocessor.size();
                            find = true;
                            break;
                        }
                    }

                    if (find)
                    { continue; }
                }
            }
        }

        if ( buffer[0] == '<' )
        {
            fprintf_s( pFile, "&lt;" );
        }
        else if ( buffer[0] == '>' )
        {
            fprintf_s( pFile, "&gt;" );
        }
        else if ( buffer[0] == '&' )
        {
            fprintf_s( pFile, "&amp;" );
        }
        else if ( buffer[0] == '\t' )
        {
            fprintf_s( pFile, "    " );
        }
        else
        {
            fputc( buffer[0], pFile );
        }

        if ( buffer[0] == '\n' )
        {
            isNewLine = true;
        }

        buffer++;
    }

    // フッタ書き込み.
    WriteFooter( pFile );

    // 出力ファイルを閉じる.
    fclose( pFile );
    pFile = nullptr;

    // バッファを解放
    free( pBuffer );
    pBuffer = nullptr;

    // 正常終了.
    return true;
}



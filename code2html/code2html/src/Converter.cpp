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
//      �L���Ȏ��ł��邩�ǂ����`�F�b�N���܂�.
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
//      �w�b�_���������݂܂�.
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
//      �t�b�^���������݂܂�.
//-------------------------------------------------------------------------------------------------
void Converter::WriteFooter(FILE* pFile)
{
    fprintf_s( pFile, "</pre>\n" );
    fprintf_s( pFile, "</body>\n" );
    fprintf_s( pFile, "</html>\n" );
}

//-------------------------------------------------------------------------------------------------
//      �L�[���[�h��ǂݍ��݂܂�.
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
//      �v���v���Z�b�T��ǂݍ��݂܂�.
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
//      �ϊ��������s���܂�.
//-------------------------------------------------------------------------------------------------
bool Converter::Process(const Config& config)
{
    // �������`�F�b�N.
    if ( config.InputFile.empty() )
    {
        printf_s( "Error : ���̓t�@�C�����ݒ肳��Ă��܂���.\n" );
        return false;
    }

    if ( config.OutputFile.empty() )
    {
        printf_s( "Error : �o�̓t�@�C�����ݒ肳��Ă��܂���.\n" );
        return false;
    }

    // ���̓t�@�C�����J��.
    FILE* pFile;
    auto err = fopen_s( &pFile, config.InputFile.c_str(), "r" );
    if ( err != 0 )
    {
        printf_s( "Error : ���̓t�@�C�����J���܂���ł���.\n" );
        return false;
    }

    fpos_t start, end;
    auto ret = fgetpos( pFile, &start );
    if ( ret != 0 )
    {
        printf_s( "Error : �t�@�C���T�C�Y�̎Z�o�Ɏ��s���܂���. ErrorCode = 1\n" );
        fclose( pFile );
        return false;
    }
    fseek( pFile, 0, SEEK_END );
    ret = fgetpos( pFile, &end );
    if ( ret != 0 )
    {
        printf_s( "Error : �t�@�C���T�C�Y�̎Z�o�Ɏ��s���܂���. ErrorCode = 2\n" );
        fclose( pFile );
        return false;
    }
    fseek( pFile, 0, SEEK_SET );

    if ( start == -1 || end == -1 )
    {
        printf_s( "Error : �t�@�C���T�C�Y�̎Z�o�Ɏ��s���܂���. ErrorCode = 3\n" );
        fclose( pFile );
        return false;
    }

    // �t�@�C���T�C�Y���Z�o����.
    auto size = static_cast<int>(end - start);

    // �o�b�t�@�̃��������m��.
    auto pBuffer = malloc( size + 1 );
    if ( pBuffer == nullptr )
    {
        printf_s( "Error : �������m�ۂɎ��s���܂���.\n" );
        fclose( pFile );
        return false;
    }

    // NULL�I�[��.
    memset( pBuffer, '\0', size + 1 );

    // �ꊇ�ǂݍ���.
    fread( pBuffer, size, 1, pFile );

    // ���̓t�@�C�������.
    fclose( pFile );

    // �o�̓t�@�C�����J��.
    err = fopen_s( &pFile, config.OutputFile.c_str(), "w" );
    if ( err != 0 )
    {
        printf_s( "Error : �o�̓t�@�C�����J���܂���ł���.\n" );
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

    // ����ʂ̃L�[���[�h�C�v���v���Z�b�T�̒P���ǂݍ���.
    std::vector<std::string> keywordList;
    std::vector<std::string> preprocessorList;
    LoadKeyword( config.Type, keywordList );
    LoadPreprocessor( config.Type, preprocessorList );

    // �擪�|�C���^��ݒ�.
    auto buffer = static_cast<char*>(pBuffer);

    // �w�b�_��������.
    WriteHeader( pFile, config.Type );

    // �\�[�X�R�[�h��ϊ�.
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
            // C�X�^�C���R�����g �J�n.
            if ( buffer[0] == '/' && buffer[1] == '*' )
            {
                if ( commentCount == 0 && singleCommentCount == 0 )
                { fprintf_s( pFile, "<font color=\'%s\'>/*", config.CommentColor.c_str() ); }
                commentCount++;
                buffer += 2;
                continue;;
            }
            // C�X�^�C���R�����g �I��.
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

            // C++�X�^�C���R�����g
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

            // XML�X�^�C���R�����g.
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
            // ������.
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

            // ����.
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
            // ##__VA_ARGS__�̂悤�Ȃ��̂��͂���.
            if ( buffer[0] == '#' && buffer[1] == '#' )
            {
                fprintf_s( pFile, "##" );
                buffer += 2;
                continue;
            }

            // #define �Ȃǂ̃v���v���Z�b�T.
            if ( buffer[0] == '#' && buffer[1] != '#' )
            {
                fprintf_s( pFile, "<font color=\'%s\'>#</font>", config.KeywordColor.c_str() );
                buffer++;
                isPreprocessor = true;
                continue;
            }
            else
            {
                // �L�[���[�h������.
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

                // ���������玟��.
                if (find)
                { continue; }

                // #�Ŏn�܂��Ă���ꍇ�� define, include�Ȃǂ̒P�ꂪ�Y�����邩�`�F�b�N.
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

    // �t�b�^��������.
    WriteFooter( pFile );

    // �o�̓t�@�C�������.
    fclose( pFile );
    pFile = nullptr;

    // �o�b�t�@�����
    free( pBuffer );
    pBuffer = nullptr;

    // ����I��.
    return true;
}



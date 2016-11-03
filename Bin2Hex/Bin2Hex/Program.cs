#region File Description
//--------------------------------------------------------------------------
// File : Program.cs
// Desc : Asura Binary To Hex Data Converter
// Copyright(c) Project Asura. All right reserved.
//--------------------------------------------------------------------------
#endregion

#region Using Statements
//--------------------------------------------------------------------------
// Using Statements
//--------------------------------------------------------------------------
using System;
using System.IO;
#endregion

namespace asura
{
    class Program
    {
        #region Fileds
        //------------------------------------------------------------------
        // Fields
        //------------------------------------------------------------------
        private static string inputFileName = null;
        private static string outputFileName = null;
        private static string description = null;
        private static string copyright = null;
        #endregion

        /// <summary>
        /// メインエントリーポイント.
        /// </summary>
        /// <param name="args">コマンドライン引数.</param>
        static void Main(string[] args)
        {
            // コマンドライン引数が1以下の場合.
            if (args.Length <= 1)
            {
                // ヘルプを表示して終了.
                ShowHelp();
            }
            else
            {
                // コマンドライン引数をチェック.
                for (int i = 0; i < args.Length; ++i)
                {
                    // 入力ファイル名.
                    if (args[i] == "-i")
                    {
                        if (args.Length > (i + 1))
                        {
                            inputFileName = args[i + 1];
                        }
                    }
                    // 出力ファイル名.
                    else if (args[i] == "-o")
                    {
                        if (args.Length > (i + 1))
                        {
                            outputFileName = args[i + 1];
                        }
                    }
                    // ファイル概要文.
                    else if (args[i] == "-d")
                    {
                        if (args.Length > (i + 1))
                        {
                            description = args[i + 1];
                        }
                    }
                    // コピーライト文.
                    else if (args[i] == "-c")
                    {
                        if (args.Length > (i + 1))
                        {
                            copyright = args[i + 1];
                        }
                    }
                }

                // 入力ファイル名と出力ファイル名が設定されていたら変換を実行.
                if ((inputFileName != null) && (outputFileName != null))
                {
                    if (!Bin2Hex())
                    {
                        // 変換に失敗.
                        Console.WriteLine("[Error] Convert Failed. input filename = {0}, output filename = {1}", inputFileName, outputFileName);
                    }
                    else
                    {
                        // 変換に成功.
                        Console.WriteLine("[Info] Convert Success. output filename = {0}", outputFileName);
                    }

                    // 設定された値をリセット.
                    inputFileName = null;
                    outputFileName = null;
                    description = null;
                    copyright = null;
                }
                else
                {
                    // エラーログ出力.
                    Console.WriteLine("[Error] Invalid Argument.");
                    if (inputFileName == null)
                    {
                        // 入力ファイル名が設定されてないよ！
                        Console.WriteLine("[Error] Input FileName is null.");
                    }
                    if (outputFileName == null)
                    {
                        // 出力ファイル名が設定されてないよ！
                        Console.WriteLine("[Error] Output FileName is null.");
                    }
                }

            }
        }

        /// <summary>
        /// バイナリデータから16進数データに変換.
        /// </summary>
        /// <returns>変換成功したらtrueを返却.</returns>
        static bool Bin2Hex()
        {
            // ファイルの存在チェック.
            if (!File.Exists(inputFileName))
            {
                Console.WriteLine("[Error] Input File Not Exists. filename = " + inputFileName);
                return false;
            }

            try
            {
                // 入力ファイルを開く.
                FileStream ifs = new FileStream(
                    inputFileName,
                    FileMode.Open,
                    FileAccess.Read);

                // 読み取りバッファを確保.
                byte[] buf = new byte[ifs.Length];

                // 一括読み込み.
                ifs.Read(buf, 0, buf.Length);

                // 入力ファイルを閉じる.
                ifs.Close();
                ifs = null;

                // 出力ファイルを作成.
                StreamWriter ofs = new StreamWriter(outputFileName);

                // ファイルヘッダーを書き込み.
                ofs.WriteLine("//---------------------------------------------------------------------------------------------------");
                ofs.WriteLine("// File : {0}", outputFileName);
                if (description != null)
                { ofs.WriteLine("// Desc : {0}", description); }
                if (copyright != null)
                { ofs.WriteLine("// Copyright(c) {0}", copyright); }
                ofs.WriteLine("//---------------------------------------------------------------------------------------------------");
                ofs.WriteLine();

                // 16進数形式に変換しながらファイルに書き込み.
                for (int i = 0; i < buf.Length; ++i)
                {
                    // 16個書き込むごとに改行する.
                    if ((i != 0) && (i % 15 == 0))
                    {
                        ofs.WriteLine();
                    }
                    else if (i % 15 != 0)
                    {
                        ofs.Write(" ");
                    }

                    ofs.Write(string.Format("0x{0, 0:X2}", buf[i]));

                    // 最後のデータでなければ，カンマを付ける.
                    if (i != (buf.Length - 1))
                    {
                        ofs.Write(",");
                    }
                }

                // 改行.
                ofs.WriteLine();

                // データサイズを書き込み.
                ofs.WriteLine("// Data Count = {0}", buf.Length);

                // 出力ファイルを閉じる.
                ofs.Close();
                ofs = null;
                buf = null;
            }
            catch (Exception e)
            {
                // 例外が発生したらエラーログを出力.
                Console.WriteLine("[Error] Exception Occurred. msg = " + e.ToString());
                return false;
            }

            // 正常終了.
            return true;
        }

        /// <summary>
        /// ヘルプを表示する.
        /// </summary>
        static void ShowHelp()
        {
            Console.WriteLine("----------------------------------------------------------");
            Console.WriteLine("   Bin2Hex.exe");
            Console.WriteLine("   Version : 1.0");
            Console.WriteLine("   Copyright(c) Project Asura. All right reserved.");
            Console.WriteLine("----------------------------------------------------------");
            Console.WriteLine("[使い方]");
            Console.WriteLine("    -i inputFileName  : [必須] 入力ファイル名を設定します.");
            Console.WriteLine("    -o outputFileName : [必須] 出力ファイル名を設定します.");
            Console.WriteLine("    -d description    : ファイル概要説明文を設定します.");
            Console.WriteLine("    -c copyright      : コピーライト文を設定します.");
            Console.WriteLine("  (例) Bin2Hex.exe -i input.txt -o output.txt");
            Console.WriteLine();
        }
    }
}

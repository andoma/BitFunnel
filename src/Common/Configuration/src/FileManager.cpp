// The MIT License (MIT)

// Copyright (c) 2016, Microsoft

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#include <fstream>

#include "BitFunnel/Configuration/Factories.h"
#include "FileManager.h"
#include "LoggerInterfaces/Logging.h"
#include "ParameterizedFile.h"


namespace BitFunnel
{
    std::unique_ptr<IFileManager>
        Factories::CreateFileManager(char const * configDirectory,
                                     char const * statisticsDirectory,
                                     char const * indexDirectory,
                                     IFileSystem & fileSystem)
    {
        return std::unique_ptr<IFileManager>(new FileManager(configDirectory,
                                                             statisticsDirectory,
                                                             indexDirectory,
                                                             fileSystem));
    }


    FileManager::FileManager(char const * /*configDirectory*/,
                             char const * statisticsDirectory,
                             char const * indexDirectory,
                             IFileSystem & fileSystem)
        : m_chunk(
            new ParameterizedFile1(fileSystem,
                                   indexDirectory,
                                   "Chunk",
                                   ".chunk")),

          m_columnDensities(new ParameterizedFile0(fileSystem,
                                                   statisticsDirectory,
                                                   "ColumnDensities",
                                                   ".csv")),
          m_columnDensitySummary(
              new ParameterizedFile0(fileSystem,
                                     statisticsDirectory,
                                     "ColumnDensitySummary",
                                     ".txt")),
          m_cumulativeTermCounts(new ParameterizedFile1(fileSystem,
                                                        statisticsDirectory,
                                                        "CumulativeTermCounts",
                                                        ".csv")),
          m_docFreqTable(new ParameterizedFile1(fileSystem,
                                                statisticsDirectory,
                                                "DocFreqTable", ".csv")),
          m_documentLengthHistogram(new ParameterizedFile0(fileSystem,
                                                           statisticsDirectory,
                                                           "DocumentLengthHistogram",
                                                           ".csv" )),
          m_indexedIdfTable(new ParameterizedFile1(fileSystem,
                                                   indexDirectory,
                                                   "IndexedIdfTable",
                                                   ".bin")),
          m_manifest(new ParameterizedFile0(fileSystem,
                                            indexDirectory,
                                            "Manifest",
                                            ".txt" )),
          m_queryPipelineStatistics(new ParameterizedFile0(fileSystem,
                                                           statisticsDirectory,
                                                           "QueryPipelineStatistics",
                                                           ".csv" )),
          m_rowDensities(
              new ParameterizedFile1(fileSystem,
                                     statisticsDirectory,
                                     "RowDensities",
                                     ".csv")),
          m_termTable(new ParameterizedFile1(fileSystem,
                                             indexDirectory,
                                             "TermTable",
                                             ".bin")),
          m_termToText(new ParameterizedFile0(fileSystem,
                                              statisticsDirectory,
                                              "TermToText",
                                              ".bin")),
          m_verificationResults(new ParameterizedFile0(fileSystem,
                                                       statisticsDirectory,
                                                       "VerificationResults",
                                                       ".csv"))
    {
    }


    //
    // FileDescriptor0 files.
    //

    FileDescriptor0 FileManager::ColumnDensities()
    {
        return FileDescriptor0(*m_columnDensities);
    }


    FileDescriptor0 FileManager::ColumnDensitySummary()
    {
        return FileDescriptor0(*m_columnDensitySummary);
    }


    FileDescriptor0 FileManager::DocumentLengthHistogram()
    {
        return FileDescriptor0(*m_documentLengthHistogram);
    }


    FileDescriptor0 FileManager::Manifest()
    {
        return FileDescriptor0(*m_manifest);
    }


    FileDescriptor0 FileManager::QueryPipelineStatistics()
    {
        return FileDescriptor0(*m_queryPipelineStatistics);
    }


    FileDescriptor0 FileManager::TermToText()
    {
        return FileDescriptor0(*m_termToText);
    }


    FileDescriptor0 FileManager::VerificationResults()
    {
        return FileDescriptor0(*m_verificationResults);
    }


    //
    // FileDescriptor1 files.
    //

    FileDescriptor1 FileManager::Chunk(size_t number)
    {
        return FileDescriptor1(*m_chunk, number);
    }


    FileDescriptor1 FileManager::CumulativeTermCounts(size_t shard)
    {
        return FileDescriptor1(*m_cumulativeTermCounts, shard);
    }


    FileDescriptor1 FileManager::DocFreqTable(size_t shard)
    {
        return FileDescriptor1(*m_docFreqTable, shard);
    }


    FileDescriptor1 FileManager::IndexedIdfTable(size_t shard)
    {
        return FileDescriptor1(*m_indexedIdfTable, shard);
    }


    FileDescriptor1 FileManager::RowDensities(size_t shard)
    {
        return FileDescriptor1(*m_rowDensities, shard);
    }


    FileDescriptor1 FileManager::TermTable(size_t shard)
    {
        return FileDescriptor1(*m_termTable, shard);
    }


    //FileDescriptor1 FileManager::DocTable(size_t shard)
    //{
    //    return FileDescriptor1(*m_docTable, shard);
    //}


    //
    // FileDescriptor2 files.
    //

    //FileDescriptor2 FileManager::IndexSlice(size_t shard, size_t slice)
    //{
    //    return FileDescriptor2(*m_indexSlice, shard, slice);
    //}
}

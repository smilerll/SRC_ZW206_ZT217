/* MediaInfoDLL - All info about media files, for DLL
// Copyright (C) 2002-2006 Jerome Martinez, Zen@MediaArea.net
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Public DLL interface implementation
// Wrapper for MediaInfo Library
// Please see MediaInfo.h for help
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef MediaInfoDLLH
#define MediaInfoDLLH

#ifndef __MINGW32__ //MinGW does not support _declspec???
    #ifdef MEDIAINFO_DLL_EXPORT
        #define MEDIAINFO_EXP extern _declspec(dllexport)
    #else
        #define MEDIAINFO_EXP extern _declspec(dllimport)
    #endif
#else //__MINGW32__
    #define MEDIAINFO_EXP
#endif //__MINGW32__

/** @brief Kinds of Stream */
typedef enum MediaInfo_stream_t
{
    MediaInfo_Stream_General,
    MediaInfo_Stream_Video,
    MediaInfo_Stream_Audio,
    MediaInfo_Stream_Text,
    MediaInfo_Stream_Chapters,
    MediaInfo_Stream_Max
} MediaInfo_stream_C;

/** @brief Kinds of Info */
typedef enum MediaInfo_info_t
{
    MediaInfo_Info_Name,
    MediaInfo_Info_Text,
    MediaInfo_Info_Measure,
    MediaInfo_Info_Options,
    MediaInfo_Info_Name_Text,
    MediaInfo_Info_Measure_Text,
    MediaInfo_Info_Info,
    MediaInfo_Info_HowTo,
    MediaInfo_Info_Max
} MediaInfo_info_C;

/** @brief Option if InfoKind = Info_Options */
typedef enum MediaInfo_infooptions_t
{
    MediaInfo_InfoOption_ShowInInform,
    MediaInfo_InfoOption_Support,
    MediaInfo_InfoOption_ShowInSupported,
    MediaInfo_InfoOption_TypeOfValue,
    MediaInfo_InfoOption_Max
} MediaInfo_infooptions_C;

/** @brief File opening options */
typedef enum MediaInfo_fileoptions_t
{
    MediaInfo_FileOption_Nothing,
    MediaInfo_FileOption_Recursive,
    MediaInfo_FileOption_Close,
    MediaInfo_FileOption_Max
} MediaInfo_fileoptions_C;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/***************************************************************************/
/*! \file MediaInfoDll.h
\brief DLL wrapper for MediaInfo.h.

DLL wrapper for MediaInfo.h \n
	Can be used for C and C++\n
    "Handle" replaces class definition
*/
/***************************************************************************/

#if defined (MEDIAINFO_DLL_EXPORT) || (defined (UNICODE) || defined (_UNICODE)) //DLL construction or Unicode
/** @brief A 'new' MediaInfo interface, return a Handle, don't forget to delete it after using it*/
MEDIAINFO_EXP void*             __stdcall MediaInfo_New (); /*you must ALWAYS call MediaInfo_Delete(Handle) in order to free memory*/
/** @brief A 'new' MediaInfo interface (with a quick init of useful options : "**VERSION**;**APP_NAME**;**APP_VERSION**", but without debug information, use it only if you know what you do), return a Handle, don't forget to delete it after using it*/
MEDIAINFO_EXP void*             __stdcall MediaInfo_New_Quick (const wchar_t* File, const wchar_t* Options); /*you must ALWAYS call MediaInfo_Delete(Handle) in order to free memory*/
/** @brief Delete a MediaInfo interface*/
MEDIAINFO_EXP void              __stdcall MediaInfo_Delete (void* Handle);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Open (with a filename)*/
MEDIAINFO_EXP int               __stdcall MediaInfo_Open (void* Handle, const wchar_t* File);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Open (with a buffer) */
MEDIAINFO_EXP int               __stdcall MediaInfo_Open_Buffer (void* Handle, const unsigned char* Begin, unsigned int Begin_Size, const unsigned char* End, unsigned int End_Size); /*return Handle*/
/** @brief Wrapper for MediaInfoLib::MediaInfo::Save */
MEDIAINFO_EXP int               __stdcall MediaInfo_Save (void* Handle);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Close */
MEDIAINFO_EXP void              __stdcall MediaInfo_Close (void* Handle);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Inform */
MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfo_Inform (void* Handle, int Reserved); /*Default : Reserved=0*/
/** @brief Wrapper for MediaInfoLib::MediaInfo::Get */
MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfo_GetI (void* Handle, MediaInfo_stream_C StreamKind, int StreamNumber, unsigned int Parameter, MediaInfo_info_C InfoKind); /*Default : InfoKind=Info_Text*/
/** @brief Wrapper for MediaInfoLib::MediaInfo::Get */
MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfo_Get (void* Handle, MediaInfo_stream_C StreamKind, int StreamNumber, const wchar_t* Parameter, MediaInfo_info_C InfoKind, MediaInfo_info_C SearchKind); /*Default : InfoKind=Info_Text, SearchKind=Info_Name*/
/** @brief Wrapper for MediaInfoLib::MediaInfo::Set */
MEDIAINFO_EXP int               __stdcall MediaInfo_SetI (void* Handle, const wchar_t* ToSet, MediaInfo_stream_C StreamKind, int StreamNumber, unsigned int Parameter, const wchar_t* OldParameter);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Set */
MEDIAINFO_EXP int               __stdcall MediaInfo_Set (void* Handle, const wchar_t* ToSet, MediaInfo_stream_C StreamKind, int StreamNumber, const wchar_t* Parameter, const wchar_t* OldParameter);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Option */
MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfo_Option (void* Handle, const wchar_t* Option, const wchar_t* Value);
/** @brief Wrapper for MediaInfoLib::MediaInfo::State_Get */
MEDIAINFO_EXP int               __stdcall MediaInfo_State_Get (void* Handle);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Count_Get */
MEDIAINFO_EXP int               __stdcall MediaInfo_Count_Get (void* Handle, MediaInfo_stream_C StreamKind, int StreamNumber); /*Default : StreamNumber=-1*/
#else //defined (MEDIAINFO_DLL_EXPORT) || (defined (UNICODE) || defined (_UNICODE))
    #define MediaInfo_New               MediaInfoA_New
    #define MediaInfo_New_Quick         MediaInfoA_New_Quick
    #define MediaInfo_Delete            MediaInfoA_Delete
    #define MediaInfo_Open              MediaInfoA_Open
    #define MediaInfo_Open_Buffer       MediaInfoA_Open_Buffer
    #define MediaInfo_Save              MediaInfoA_Save
    #define MediaInfo_Close             MediaInfoA_Close
    #define MediaInfo_Inform            MediaInfoA_Inform
    #define MediaInfo_GetI              MediaInfoA_GetI
    #define MediaInfo_Get               MediaInfoA_Get
    #define MediaInfo_SetI              MediaInfoA_SetI
    #define MediaInfo_Set               MediaInfoA_Set
    #define MediaInfo_Option            MediaInfoA_Option
    #define MediaInfo_State_Get         MediaInfoA_State_Get
    #define MediaInfo_Count_Get         MediaInfoA_Count_Get
#endif //defined (MEDIAINFO_DLL_EXPORT) || (defined (UNICODE) || defined (_UNICODE))

/** @brief A 'new' MediaInfo interface, return a Handle, don't forget to delete it after using it*/
MEDIAINFO_EXP void*             __stdcall MediaInfoA_New (); /*you must ALWAYS call MediaInfo_Delete(Handle) in order to free memory*/
/** @brief A 'new' MediaInfo interface (with a quick init of useful options : "**VERSION**;**APP_NAME**;**APP_VERSION**", but without debug information, use it only if you know what you do), return a Handle, don't forget to delete it after using it*/
MEDIAINFO_EXP void*             __stdcall MediaInfoA_New_Quick (const char* File, const char* Options); /*you must ALWAYS call MediaInfo_Delete(Handle) in order to free memory*/
/** @brief Delete a MediaInfo interface*/
MEDIAINFO_EXP void              __stdcall MediaInfoA_Delete (void* Handle);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Open (with a filename)*/
MEDIAINFO_EXP int               __stdcall MediaInfoA_Open (void* Handle, const char* File); /*you must ALWAYS call MediaInfo_Close(Handle) in order to free memory*/
/** @brief Wrapper for MediaInfoLib::MediaInfo::Open (with a buffer) */
MEDIAINFO_EXP int               __stdcall MediaInfoA_Open_Buffer (void* Handle, const unsigned char* Begin, unsigned int Begin_Size, const unsigned char* End, unsigned int End_Size); /*return Handle*/
/** @brief Wrapper for MediaInfoLib::MediaInfo::Save */
MEDIAINFO_EXP int               __stdcall MediaInfoA_Save (void* Handle);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Close */
MEDIAINFO_EXP void              __stdcall MediaInfoA_Close (void* Handle);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Inform */
MEDIAINFO_EXP const char*       __stdcall MediaInfoA_Inform (void* Handle, int Reserved); /*Default : Reserved=MediaInfo_*/
/** @brief Wrapper for MediaInfoLib::MediaInfo::Get */
MEDIAINFO_EXP const char*       __stdcall MediaInfoA_GetI (void* Handle, MediaInfo_stream_C StreamKind, int StreamNumber, unsigned int Parameter, MediaInfo_info_C InfoKind); /*Default : InfoKind=Info_Text*/
/** @brief Wrapper for MediaInfoLib::MediaInfo::Get */
MEDIAINFO_EXP const char*       __stdcall MediaInfoA_Get (void* Handle, MediaInfo_stream_C StreamKind, int StreamNumber, const char* Parameter, MediaInfo_info_C InfoKind, MediaInfo_info_C SearchKind); /*Default : InfoKind=Info_Text, SearchKind=Info_Name*/
/** @brief Wrapper for MediaInfoLib::MediaInfo::Set */
MEDIAINFO_EXP int               __stdcall MediaInfoA_SetI (void* Handle, const char* ToSet, MediaInfo_stream_C StreamKind, int StreamNumber, unsigned int Parameter, const char* OldParameter);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Set */
MEDIAINFO_EXP int               __stdcall MediaInfoA_Set (void* Handle, const char* ToSet, MediaInfo_stream_C StreamKind, int StreamNumber, const char* Parameter, const char* OldParameter);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Option */
MEDIAINFO_EXP const char*       __stdcall MediaInfoA_Option (void* Handle, const char* Option, const char* Value);
/** @brief Wrapper for MediaInfoLib::MediaInfo::State_Get */
MEDIAINFO_EXP int               __stdcall MediaInfoA_State_Get (void* Handle);
/** @brief Wrapper for MediaInfoLib::MediaInfo::Count_Get */
MEDIAINFO_EXP int               __stdcall MediaInfoA_Count_Get (void* Handle, MediaInfo_stream_C StreamKind, int StreamNumber); /*Default : StreamNumber=-1*/


#if defined (MEDIAINFO_DLL_EXPORT) || (defined (UNICODE) || defined (_UNICODE)) //DLL construction or Unicode
/** @brief A 'new' MediaInfoList interface, return a Handle, don't forget to delete it after using it*/
MEDIAINFO_EXP void*             __stdcall MediaInfoList_New (); /*you must ALWAYS call MediaInfoList_Delete(Handle) in order to free memory*/
/** @brief A 'new' MediaInfoList interface (with a quick init of useful options : "**VERSION**;**APP_NAME**;**APP_VERSION**", but without debug information, use it only if you know what you do), return a Handle, don't forget to delete it after using it*/
MEDIAINFO_EXP void*             __stdcall MediaInfoList_New_Quick (const wchar_t* Files, const wchar_t* Config); /*you must ALWAYS call MediaInfoList_Delete(Handle) in order to free memory*/
/** @brief Delete a MediaInfoList interface*/
MEDIAINFO_EXP void              __stdcall MediaInfoList_Delete (void* Handle);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Open (with a filename)*/
MEDIAINFO_EXP int               __stdcall MediaInfoList_Open (void* Handle, const wchar_t* Files, const MediaInfo_fileoptions_C Options); /*Default : Options=MediaInfo_FileOption_Nothing*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Open (with a buffer) */
MEDIAINFO_EXP int               __stdcall MediaInfoList_Open_Buffer (void* Handle, const unsigned char* Begin, unsigned int Begin_Size, const unsigned char* End, unsigned int End_Size); /*return Handle*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Save */
MEDIAINFO_EXP int               __stdcall MediaInfoList_Save (void* Handle, unsigned int FilePos);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Close */
MEDIAINFO_EXP void              __stdcall MediaInfoList_Close (void* Handle, unsigned int FilePos);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Inform */
MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfoList_Inform (void* Handle, unsigned int FilePos, int Reserved); /*Default : Reserved=0*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Get */
MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfoList_GetI (void* Handle, unsigned int FilePos, MediaInfo_stream_C StreamKind, int StreamNumber, unsigned int Parameter, MediaInfo_info_C InfoKind); /*Default : InfoKind=Info_Text*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Get */
MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfoList_Get (void* Handle, unsigned int FilePos, MediaInfo_stream_C StreamKind, int StreamNumber, const wchar_t* Parameter, MediaInfo_info_C InfoKind, MediaInfo_info_C SearchKind); /*Default : InfoKind=Info_Text, SearchKind=Info_Name*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Set */
MEDIAINFO_EXP int               __stdcall MediaInfoList_SetI (void* Handle, const wchar_t* ToSet, unsigned int FilePos, MediaInfo_stream_C StreamKind, int StreamNumber, unsigned int Parameter, const wchar_t* OldParameter);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Set */
MEDIAINFO_EXP int               __stdcall MediaInfoList_Set (void* Handle, const wchar_t* ToSet, unsigned int FilePos, MediaInfo_stream_C StreamKind, int StreamNumber, const wchar_t* Parameter, const wchar_t* OldParameter);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Option */
MEDIAINFO_EXP const wchar_t*    __stdcall MediaInfoList_Option (void* Handle, const wchar_t* Option, const wchar_t* Value);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::State_Get */
MEDIAINFO_EXP int               __stdcall MediaInfoList_State_Get (void* Handle);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Count_Get */
MEDIAINFO_EXP int               __stdcall MediaInfoList_Count_Get (void* Handle, unsigned int FilePos, MediaInfo_stream_C StreamKind, int StreamNumber); /*Default : StreamNumber=-1*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Count_Get */
MEDIAINFO_EXP int               __stdcall MediaInfoList_Count_Get_Files (void* Handle);
#else //defined (MEDIAINFO_DLL_EXPORT) || (defined (UNICODE) || defined (_UNICODE))
    #define MediaInfoList_New               MediaInfoListA_New
    #define MediaInfoList_New_Quick         MediaInfoListA_New_Quick
    #define MediaInfoList_Delete            MediaInfoListA_Delete
    #define MediaInfoList_Open              MediaInfoListA_Open
    #define MediaInfoList_Open_Buffer       MediaInfoListA_Open_Buffer
    #define MediaInfoList_Save              MediaInfoListA_Save
    #define MediaInfoList_Save_All          MediaInfoListA_Save_All
    #define MediaInfoList_Close             MediaInfoListA_Close
    #define MediaInfoList_Close_All         MediaInfoListA_Close_All
    #define MediaInfoList_Inform            MediaInfoListA_Inform
    #define MediaInfoList_Inform_All        MediaInfoListA_Inform_All
    #define MediaInfoList_GetI              MediaInfoListA_GetI
    #define MediaInfoList_Get               MediaInfoListA_Get
    #define MediaInfoList_SetI              MediaInfoListA_SetI
    #define MediaInfoList_Set               MediaInfoListA_Set
    #define MediaInfoList_Option            MediaInfoListA_Option
    #define MediaInfoList_State_Get         MediaInfoListA_State_Get
    #define MediaInfoList_Count_Get         MediaInfoListA_Count_Get
    #define MediaInfoList_Count_Get_Files   MediaInfoListA_Count_Get_Files
#endif //defined (MEDIAINFO_DLL_EXPORT) || (defined (UNICODE) || defined (_UNICODE))

/* Warning : Deprecated, use MediaInfo_Option("Info_Version", "**YOUR VERSION COMPATIBLE**") instead */
MEDIAINFO_EXP const char*       __stdcall MediaInfo_Info_Version ();


/** @brief A 'new' MediaInfoList interface, return a Handle, don't forget to delete it after using it*/
MEDIAINFO_EXP void*             __stdcall MediaInfoListA_New (); /*you must ALWAYS call MediaInfoList_Delete(Handle) in order to free memory*/
/** @brief A 'new' MediaInfoList interface (with a quick init of useful options : "**VERSION**;**APP_NAME**;**APP_VERSION**", but without debug information, use it only if you know what you do), return a Handle, don't forget to delete it after using it*/
MEDIAINFO_EXP void*             __stdcall MediaInfoListA_New_Quick (const char* Files, const char* Config); /*you must ALWAYS call MediaInfoList_Delete(Handle) in order to free memory*/
/** @brief Delete a MediaInfoList interface*/
MEDIAINFO_EXP void              __stdcall MediaInfoListA_Delete (void* Handle);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Open (with a filename)*/
MEDIAINFO_EXP int               __stdcall MediaInfoListA_Open (void* Handle, const char* Files, const MediaInfo_fileoptions_C Options); /*Default : Options=0*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Open (with a buffer) */
MEDIAINFO_EXP int               __stdcall MediaInfoListA_Open_Buffer (void* Handle, const unsigned char* Begin, unsigned int Begin_Size, const unsigned char* End, unsigned int End_Size); /*return Handle*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Save */
MEDIAINFO_EXP int               __stdcall MediaInfoListA_Save (void* Handle, unsigned int FilePos);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Close */
MEDIAINFO_EXP void              __stdcall MediaInfoListA_Close (void* Handle, unsigned int FilePos);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Inform */
MEDIAINFO_EXP const char*       __stdcall MediaInfoListA_Inform (void* Handle, unsigned int FilePos, int Reserved); /*Default : Reserved=0*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Get */
MEDIAINFO_EXP const char*       __stdcall MediaInfoListA_GetI (void* Handle, unsigned int FilePos, MediaInfo_stream_C StreamKind, int StreamNumber, unsigned int Parameter, MediaInfo_info_C InfoKind); /*Default : InfoKind=Info_Text*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Get */
MEDIAINFO_EXP const char*       __stdcall MediaInfoListA_Get (void* Handle, unsigned int FilePos, MediaInfo_stream_C StreamKind, int StreamNumber, const char* Parameter, MediaInfo_info_C InfoKind, MediaInfo_info_C SearchKind); /*Default : InfoKind=Info_Text, SearchKind=Info_Name*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Set */
MEDIAINFO_EXP int               __stdcall MediaInfoListA_SetI (void* Handle, const char* ToSet, unsigned int FilePos, MediaInfo_stream_C StreamKind, int StreamNumber, unsigned int Parameter, const char* OldParameter);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Set */
MEDIAINFO_EXP int               __stdcall MediaInfoListA_Set (void* Handles, const char* ToSet, unsigned int FilePos, MediaInfo_stream_C StreamKind, int StreamNumber, const char* Parameter, const char* OldParameter);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Option */
MEDIAINFO_EXP const char*       __stdcall MediaInfoListA_Option (void* Handle, const char* Option, const char* Value);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::State_Get */
MEDIAINFO_EXP int               __stdcall MediaInfoListA_State_Get (void* Handle);
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Count_Get */
MEDIAINFO_EXP int               __stdcall MediaInfoListA_Count_Get (void* Handle, unsigned int FilePos, MediaInfo_stream_C StreamKind, int StreamNumber); /*Default : StreamNumber=-1*/
/** @brief Wrapper for MediaInfoListLib::MediaInfoList::Count_Get */
MEDIAINFO_EXP int               __stdcall MediaInfoListA_Count_Get_Files (void* Handle);

#ifdef __cplusplus
}
#endif /*__cplusplus*/



#ifdef __cplusplus
//DLL C++ wrapper for C functions
#if !defined(MediaInfoH) && !defined (MEDIAINFO_DLL_EXPORT) //No Lib include and No DLL construction

//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
//Char types
#undef  _T
#define _T(__x)     __T(__x)
#if defined(UNICODE) || defined (_UNICODE)
    typedef wchar_t Char;
    #undef  __T
    #define __T(__x) L ## __x
#else
    typedef char Char;
    #undef  __T
    #define __T(__x) __x
#endif
typedef std::basic_string<Char, std::char_traits<Char>, std::allocator<Char> > MediaInfo_String;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/// @brief Kinds of Stream
enum stream_t
{
    Stream_General,                 ///< StreamKind = General
    Stream_Video,                   ///< StreamKind = Video
    Stream_Audio,                   ///< StreamKind = Audio
    Stream_Text,                    ///< StreamKind = Text
    Stream_Chapters,                ///< StreamKind = Chapters
    Stream_Image,                   ///< StreamKind = Image
    Stream_Max,
};

/// @brief Kind of information
enum info_t
{
    Info_Name,                      ///< InfoKind = Unique name of parameter
    Info_Text,                      ///< InfoKind = Value of parameter
    Info_Measure,                   ///< InfoKind = Unique name of measure unit of parameter
    Info_Options,                   ///< InfoKind = See infooptions_t
    Info_Name_Text,                 ///< InfoKind = Translated name of parameter
    Info_Measure_Text,              ///< InfoKind = Translated name of measure unit
    Info_Info,                      ///< InfoKind = More information about the parameter
    Info_HowTo,                     ///< InfoKind = Information : how data is found
    Info_Max
};

/// Get(...)[infooptions_t] return a string like "YNYN..." \n
/// Use this enum to know at what correspond the Y (Yes) or N (No)
/// If Get(...)[0]==Y, then :
/// @brief Option if InfoKind = Info_Options
enum infooptions_t
{
    InfoOption_ShowInInform,		///< Show this parameter in Inform()
    InfoOption_Support,				///< how this parameter is supported, could be N (No), R (Read only), W (Read/Write), space (Non applicable)
    InfoOption_ShowInSupported,		///< Internal use only (info : Must be showed in Info_Capacities() )
    InfoOption_Max
};

//---------------------------------------------------------------------------
/// @brief File opening options
enum fileoptions_t
{
    FileOption_Nothing,
    FileOption_Recursive,           ///< Browse folders recursively
    FileOption_Close,               ///< Close all files before open
    FileOption_Max
};
//---------------------------------------------------------------------------

class MediaInfo
{
public :
    MediaInfo ()                {Handle=MediaInfo_New();};
    ~MediaInfo ()               {MediaInfo_Close(Handle);};

    //File
    int Open (const MediaInfo_String &File) {return MediaInfo_Open(Handle, File.c_str());};
    int Open (const unsigned char* Begin, size_t Begin_Size, const unsigned char* End=NULL, size_t End_Size=NULL) {return MediaInfo_Open_Buffer(Handle, Begin, Begin_Size, End, End_Size);};
    int Save () {return MediaInfo_Save(Handle);};
    void Close () {return MediaInfo_Close(Handle);};

    //General information
    MediaInfo_String Inform (int Reserved=NULL)  {return MediaInfo_Inform(Handle, NULL);};
    MediaInfo_String Get (stream_t StreamKind, int StreamNumber, int Parameter, info_t InfoKind=Info_Text)  {return MediaInfo_GetI (Handle, (MediaInfo_stream_C)StreamKind, StreamNumber, Parameter, (MediaInfo_info_C)InfoKind);};
    MediaInfo_String Get (stream_t StreamKind, int StreamNumber, const MediaInfo_String &Parameter, info_t InfoKind=Info_Text, info_t SearchKind=Info_Name)  {return MediaInfo_Get (Handle, (MediaInfo_stream_C)StreamKind, StreamNumber, Parameter.c_str(), (MediaInfo_info_C)InfoKind, (MediaInfo_info_C)SearchKind);};
    int Set (const MediaInfo_String &ToSet, stream_t StreamKind, int StreamNumber, int Parameter, const MediaInfo_String &OldValue=_T(""))  {return MediaInfo_SetI (Handle, ToSet.c_str(), (MediaInfo_stream_C)StreamKind, StreamNumber, Parameter, OldValue.c_str());};
    int Set (const MediaInfo_String &ToSet, stream_t StreamKind, int StreamNumber, const MediaInfo_String &Parameter, const MediaInfo_String &OldValue=_T(""))  {return MediaInfo_Set (Handle, ToSet.c_str(), (MediaInfo_stream_C)StreamKind, StreamNumber, Parameter.c_str(), OldValue.c_str());};
    MediaInfo_String        Option (const MediaInfo_String &Option, const MediaInfo_String &Value=_T(""))  {return MediaInfo_Option (Handle, Option.c_str(), Value.c_str());};
    static MediaInfo_String Option_Static (const MediaInfo_String &Option, const MediaInfo_String &Value=_T(""))  {return MediaInfo_Option (NULL, Option.c_str(), Value.c_str());};
    int                     State_Get ()  {return MediaInfo_State_Get(Handle);};
    int                     Count_Get (stream_t StreamKind, int StreamNumber=-1)  {return MediaInfo_Count_Get(Handle, (MediaInfo_stream_C)StreamKind, StreamNumber);};

private :
    void* Handle;
};

class MediaInfoList
{
public :
    MediaInfoList ()                {Handle=MediaInfoList_New();};
    ~MediaInfoList ()               {MediaInfoList_Delete(Handle);};

    //File
    int Open (const MediaInfo_String &File, const fileoptions_t Options=FileOption_Nothing) {return MediaInfoList_Open(Handle, File.c_str(), (MediaInfo_fileoptions_C)Options);};
    int Open (const unsigned char* Begin, size_t Begin_Size, const unsigned char* End=NULL, size_t End_Size=NULL) {return MediaInfoList_Open_Buffer(Handle, Begin, Begin_Size, End, End_Size);};
    int Save (unsigned int FilePos) {return MediaInfoList_Save(Handle, FilePos);};
    void Close (unsigned int FilePos) {return MediaInfoList_Close(Handle, FilePos);};

    //General information
    MediaInfo_String Inform (unsigned int FilePos=-1, int Reserved=NULL)  {return MediaInfoList_Inform(Handle, FilePos, NULL);};
    MediaInfo_String Get (unsigned int FilePos, stream_t StreamKind, int StreamNumber, int Parameter, info_t InfoKind=Info_Text)  {return MediaInfoList_GetI (Handle, FilePos, (MediaInfo_stream_C)StreamKind, StreamNumber, Parameter, (MediaInfo_info_C)InfoKind);};
    MediaInfo_String Get (unsigned int FilePos, stream_t StreamKind, int StreamNumber, const MediaInfo_String &Parameter, info_t InfoKind=Info_Text, info_t SearchKind=Info_Name)  {return MediaInfoList_Get (Handle, FilePos, (MediaInfo_stream_C)StreamKind, StreamNumber, Parameter.c_str(), (MediaInfo_info_C)InfoKind, (MediaInfo_info_C)SearchKind);};
    int Set (const MediaInfo_String &ToSet, unsigned int FilePos, stream_t StreamKind, int StreamNumber, int Parameter, const MediaInfo_String &OldValue=_T(""))  {return MediaInfoList_SetI (Handle, ToSet.c_str(), FilePos, (MediaInfo_stream_C)StreamKind, StreamNumber, Parameter, OldValue.c_str());};
    int Set (const MediaInfo_String &ToSet, unsigned int FilePos, stream_t StreamKind, int StreamNumber, const MediaInfo_String &Parameter, const MediaInfo_String &OldValue=_T(""))  {return MediaInfoList_Set (Handle, ToSet.c_str(), FilePos, (MediaInfo_stream_C)StreamKind, StreamNumber, Parameter.c_str(), OldValue.c_str());};
    MediaInfo_String        Option (const MediaInfo_String &Option, const MediaInfo_String &Value=_T(""))  {return MediaInfoList_Option (Handle, Option.c_str(), Value.c_str());};
    static MediaInfo_String Option_Static (const MediaInfo_String &Option, const MediaInfo_String &Value=_T(""))  {return MediaInfoList_Option (NULL, Option.c_str(), Value.c_str());};
    int                     State_Get ()  {return MediaInfoList_State_Get(Handle);};
    int                     Count_Get (unsigned int FilePos, stream_t StreamKind, int StreamNumber=-1)  {return MediaInfoList_Count_Get(Handle, FilePos, (MediaInfo_stream_C)StreamKind, StreamNumber);};
    int                     Count_Get ()  {return MediaInfoList_Count_Get_Files(Handle);};

private :
    void* Handle;
};
} //NameSpace
#endif//#if !defined(MediaInfoH) && !defined (MEDIAINFO_DLL_EXPORT) && !(defined (UNICODE) || defined (_UNICODE))
#endif /*__cplusplus*/


#endif

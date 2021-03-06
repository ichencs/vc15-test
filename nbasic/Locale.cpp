#include "Locale.h"
#if defined VCZH_MSVC
#include <Windows.h>
#elif defined VCZH_GCC
#include <stdio.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>
#endif

namespace vl
{
	using namespace collections;
	
#if defined VCZH_MSVC
	
	extern SYSTEMTIME DateTimeToSystemTime(const DateTime& dateTime);
	
	BOOL CALLBACK Locale_EnumLocalesProcEx(
	  _In_  LPWSTR lpLocaleString,
	  _In_  DWORD dwFlags,
	  _In_  LPARAM lParam
	)
	{
		((List<Locale>*)lParam)->Add(Locale(lpLocaleString));
		return TRUE;
	}
	
	BOOL CALLBACK Locale_EnumDateFormatsProcExEx(
	  _In_  LPWSTR lpDateFormatString,
	  _In_  CALID CalendarID,
	  _In_  LPARAM lParam
	)
	{
		((List<WString>*)lParam)->Add(lpDateFormatString);
		return TRUE;
	}
	
	BOOL CALLBACK EnumTimeFormatsProcEx(
	  _In_  LPWSTR lpTimeFormatString,
	  _In_  LPARAM lParam
	)
	{
		((List<WString>*)lParam)->Add(lpTimeFormatString);
		return TRUE;
	}
	
	WString Transform(const WString& localeName, const WString& input, DWORD flag)
	{
		int length = LCMapStringEx(localeName.Buffer(), flag, input.Buffer(), (int)input.Length() + 1, NULL, 0, NULL, NULL, NULL);
		Array<wchar_t> buffer(length);
		LCMapStringEx(localeName.Buffer(), flag, input.Buffer(), (int)input.Length() + 1, &buffer[0], (int)buffer.Count(), NULL, NULL, NULL);
		return &buffer[0];
	}
	
	DWORD TranslateNormalization(Locale::Normalization normalization)
	{
		DWORD result = 0;
		
		if (normalization & Locale::IgnoreCase)
		{
			result |= NORM_IGNORECASE;
		}
		
		if (normalization & Locale::IgnoreCaseLinguistic)
		{
			result |= NORM_IGNORECASE | NORM_LINGUISTIC_CASING;
		}
		
		if (normalization & Locale::IgnoreKanaType)
		{
			result |= NORM_IGNOREKANATYPE;
		}
		
		if (normalization & Locale::IgnoreNonSpace)
		{
			result |= NORM_IGNORENONSPACE;
		}
		
		if (normalization & Locale::IgnoreSymbol)
		{
			result |= NORM_IGNORESYMBOLS;
		}
		
		if (normalization & Locale::IgnoreWidth)
		{
			result |= NORM_IGNOREWIDTH;
		}
		
#if _MSC_VER > 1700
		
		if (normalization & Locale::DigitsAsNumbers)
		{
			result |= SORT_DIGITSASNUMBERS;
		}
		
#endif
		
		if (normalization & Locale::StringSoft)
		{
			result |= SORT_STRINGSORT;
		}
		
		return result;
	}
	
#endif
	
	/***********************************************************************
	Locale
	***********************************************************************/
	
	Locale::Locale(const WString& _localeName)
		: localeName(_localeName)
	{
	}
	
	Locale::~Locale()
	{
	}
	
	Locale Locale::Invariant()
	{
#if defined VCZH_MSVC
		return Locale(LOCALE_NAME_INVARIANT);
#elif defined VCZH_GCC
		return Locale(L"");
#endif
	}
	
	Locale Locale::SystemDefault()
	{
		wchar_t buffer[LOCALE_NAME_MAX_LENGTH + 1] = {0};
		GetSystemDefaultLocaleName(buffer, LOCALE_NAME_MAX_LENGTH);
		return Locale(buffer);
	}
	
	Locale Locale::UserDefault()
	{
		wchar_t buffer[LOCALE_NAME_MAX_LENGTH + 1] = {0};
		GetUserDefaultLocaleName(buffer, LOCALE_NAME_MAX_LENGTH);
		return Locale(buffer);
	}
	
	void Locale::Enumerate(collections::List<Locale>& locales)
	{
		EnumSystemLocalesEx(&Locale_EnumLocalesProcEx, LOCALE_ALL, (LPARAM)&locales, NULL);
	}
	
	const WString& Locale::GetName()const
	{
		return localeName;
	}
	
	void Locale::GetShortDateFormats(collections::List<WString>& formats)const
	{
		EnumDateFormatsExEx(&Locale_EnumDateFormatsProcExEx, localeName.Buffer(), DATE_SHORTDATE, (LPARAM)&formats);
	}
	
	void Locale::GetLongDateFormats(collections::List<WString>& formats)const
	{
		EnumDateFormatsExEx(&Locale_EnumDateFormatsProcExEx, localeName.Buffer(), DATE_LONGDATE, (LPARAM)&formats);
	}
	
	void Locale::GetYearMonthDateFormats(collections::List<WString>& formats)const
	{
		EnumDateFormatsExEx(&Locale_EnumDateFormatsProcExEx, localeName.Buffer(), DATE_YEARMONTH, (LPARAM)&formats);
	}
	
	void Locale::GetLongTimeFormats(collections::List<WString>& formats)const
	{
		EnumTimeFormatsEx(&EnumTimeFormatsProcEx, localeName.Buffer(), 0, (LPARAM)&formats);
	}
	
	void Locale::GetShortTimeFormats(collections::List<WString>& formats)const
	{
		EnumTimeFormatsEx(&EnumTimeFormatsProcEx, localeName.Buffer(), TIME_NOSECONDS, (LPARAM)&formats);
	}
	
	WString Locale::FormatDate(const WString& format, DateTime date)const
	{
		SYSTEMTIME st = DateTimeToSystemTime(date);
		int length = GetDateFormatEx(localeName.Buffer(), 0, &st, format.Buffer(), NULL, 0, NULL);
		
		if (length == 0)
		{
			return L"";
		}
		
		Array<wchar_t> buffer(length);
		GetDateFormatEx(localeName.Buffer(), 0, &st, format.Buffer(), &buffer[0], (int)buffer.Count(), NULL);
		return &buffer[0];
	}
	
	WString Locale::FormatTime(const WString& format, DateTime time)const
	{
#if defined VCZH_MSVC
		SYSTEMTIME st = DateTimeToSystemTime(time);
		int length = GetTimeFormatEx(localeName.Buffer(), 0, &st, format.Buffer(), NULL, 0);
		
		if (length == 0)
		{
			return L"";
		}
		
		Array<wchar_t> buffer(length);
		GetTimeFormatEx(localeName.Buffer(), 0, &st, format.Buffer(), &buffer[0], (int)buffer.Count());
		return &buffer[0];
		// #elif defined VCZH_GCC
		// 		return FormatDate(format, time);
#endif
	}
	
#ifdef VCZH_MSVC
	WString Locale::FormatNumber(const WString& number)const
	{
		int length = GetNumberFormatEx(localeName.Buffer(), 0, number.Buffer(), NULL, NULL, 0);
		
		if (length == 0)
		{
			return L"";
		}
		
		Array<wchar_t> buffer(length);
		GetNumberFormatEx(localeName.Buffer(), 0, number.Buffer(), NULL, &buffer[0], (int)buffer.Count());
		return &buffer[0];
	}
	
	WString Locale::FormatCurrency(const WString& currency)const
	{
		int length = GetCurrencyFormatEx(localeName.Buffer(), 0, currency.Buffer(), NULL, NULL, 0);
		
		if (length == 0)
		{
			return L"";
		}
		
		Array<wchar_t> buffer(length);
		GetCurrencyFormatEx(localeName.Buffer(), 0, currency.Buffer(), NULL, &buffer[0], (int)buffer.Count());
		return &buffer[0];
	}
#endif
	
	WString Locale::GetShortDayOfWeekName(vint dayOfWeek)const
	{
		return FormatDate(L"ddd", DateTime::FromDateTime(2000, 1, 2 + dayOfWeek));
	}
	
	WString Locale::GetLongDayOfWeekName(vint dayOfWeek)const
	{
		return FormatDate(L"dddd", DateTime::FromDateTime(2000, 1, 2 + dayOfWeek));
	}
	
	WString Locale::GetShortMonthName(vint month)const
	{
		return FormatDate(L"MMM", DateTime::FromDateTime(2000, month, 1));
	}
	
	WString Locale::GetLongMonthName(vint month)const
	{
		return FormatDate(L"MMMM", DateTime::FromDateTime(2000, month, 1));
	}
	
#ifdef VCZH_MSVC
	WString Locale::ToFullWidth(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_FULLWIDTH);
	}
	
	WString Locale::ToHalfWidth(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_HALFWIDTH);
	}
	
	WString Locale::ToHiragana(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_HIRAGANA);
	}
	
	WString Locale::ToKatagana(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_KATAKANA);
	}
#endif
	
	WString Locale::ToLower(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_LOWERCASE);
	}
	
	WString Locale::ToUpper(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_UPPERCASE);
	}
	
	WString Locale::ToLinguisticLower(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_LOWERCASE | LCMAP_LINGUISTIC_CASING);
	}
	
	WString Locale::ToLinguisticUpper(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_UPPERCASE | LCMAP_LINGUISTIC_CASING);
	}
	
#ifdef VCZH_MSVC
	WString Locale::ToSimplifiedChinese(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_SIMPLIFIED_CHINESE);
	}
	
	WString Locale::ToTraditionalChinese(const WString& str)const
	{
		return Transform(localeName, str, LCMAP_TRADITIONAL_CHINESE);
	}
	
	WString Locale::ToTileCase(const WString& str)const
	{
#if _MSC_VER > 1700
	
		return Transform(localeName, str, LCMAP_TITLECASE);
#else
		return WString();
#endif
	}
#endif
	
	vint Locale::Compare(const WString& s1, const WString& s2, Normalization normalization)const
	{
	
		switch (CompareStringEx(localeName.Buffer(), TranslateNormalization(normalization), s1.Buffer(), (int)s1.Length(), s2.Buffer(), (int)s2.Length(), NULL, NULL, NULL))
		{
			case CSTR_LESS_THAN: return -1;
			
			case CSTR_GREATER_THAN: return 1;
			
			default: return 0;
		}
	}
	
	vint Locale::CompareOrdinal(const WString& s1, const WString& s2)const
	{
#if defined VCZH_MSVC
	
		switch (CompareStringOrdinal(s1.Buffer(), (int)s1.Length(), s2.Buffer(), (int)s2.Length(), FALSE))
		{
			case CSTR_LESS_THAN: return -1;
			
			case CSTR_GREATER_THAN: return 1;
			
			default: return 0;
		}
		
#elif defined VCZH_GCC
		return wcscmp(s1.Buffer(), s2.Buffer());
#endif
	}
	
	vint Locale::CompareOrdinalIgnoreCase(const WString& s1, const WString& s2)const
	{
#if defined VCZH_MSVC
	
		switch (CompareStringOrdinal(s1.Buffer(), (int)s1.Length(), s2.Buffer(), (int)s2.Length(), TRUE))
		{
			case CSTR_LESS_THAN: return -1;
			
			case CSTR_GREATER_THAN: return 1;
			
			default: return 0;
		}
		
#elif defined VCZH_GCC
		return wcscasecmp(s1.Buffer(), s2.Buffer());
#endif
	}
	
	collections::Pair<vint, vint> Locale::FindFirst(const WString& text, const WString& find, Normalization normalization)const
	{
		int length = 0;
		int result = FindNLSStringEx(localeName.Buffer(), FIND_FROMSTART | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), &length, NULL, NULL, NULL);
		return result == -1 ? Pair<vint, vint>(-1, 0) : Pair<vint, vint>(result, length);
	}
	
	collections::Pair<vint, vint> Locale::FindLast(const WString& text, const WString& find, Normalization normalization)const
	{
		int length = 0;
		int result = FindNLSStringEx(localeName.Buffer(), FIND_FROMEND | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), &length, NULL, NULL, NULL);
		return result == -1 ? Pair<vint, vint>(-1, 0) : Pair<vint, vint>(result, length);
	}
	
	bool Locale::StartsWith(const WString& text, const WString& find, Normalization normalization)const
	{
		int result = FindNLSStringEx(localeName.Buffer(), FIND_STARTSWITH | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), NULL, NULL, NULL, NULL);
		return result != -1;
	}
	
	bool Locale::EndsWith(const WString& text, const WString& find, Normalization normalization)const
	{
		int result = FindNLSStringEx(localeName.Buffer(), FIND_ENDSWITH | TranslateNormalization(normalization), text.Buffer(), (int)text.Length(), find.Buffer(), (int)find.Length(), NULL, NULL, NULL, NULL);
		return result != -1;
	}
}

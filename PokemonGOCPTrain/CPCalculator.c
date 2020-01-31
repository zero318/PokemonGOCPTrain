#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define AbsMinPokemon 0
#define AbsMaxPokemon 963
#define AbsPokemonCount ((AbsMaxPokemon - AbsMinPokemon) + 1)
#define AbsMinLevel 0
#define AbsMaxLevel 88
#define AbsLevelCount ((AbsMaxLevel - AbsMinLevel) + 1)
#define MinIV 0
#define MaxIV 15
#define IVCount ((MaxIV - MinIV) + 1)
#define ExcelMaxColumns 16384
#define MinCPM 0
#define MaxCPM 44
#define CPMCount ((MaxCPM - MinCPM) + 1)
#define AbsMinPercentIV 0.0
#define AbsMaxPercentIV 1.0

#define DoSomething(OtherMacro) do {OtherMacro} while (0)
#define DoNothing do {} while (0)

#define FlushStdIn() while ((getchar()) != '\n')

#define VerboseMode Verbose
#define VerboseDigits VerboseLength
#define DenominatorName VerboseDenominator
#define NumeratorName VerboseNumerator

//This macro is literally just a toggleable printf.
//The syntax is the same as the regular printf, but
//it can be toggled on/off by defining PrintCrap
#define PrintCrap

#ifdef PrintCrap
#define CrappyPrintf(...) DoSomething(printf(__VA_ARGS__);)
#define VerboseMessage(Label) DoSomething(\
if (VerboseMode) {\
	printf(Label);\
})
#define VerboseProgressHeader(Label, Length) DoSomething(\
if (VerboseMode) {\
	printf(Label);\
	VerboseDigits = (char)(floor(log10(Length) + 1) + 48); \
	DenominatorName[1] = VerboseDigits + 1; \
	NumeratorName[2] = VerboseDigits; \
	printf(DenominatorName, '/', Length);\
})
#define VerboseProgress(Value) DoSomething(\
if (VerboseMode) {\
	printf(NumeratorName, Value);\
})
#else
#define CrappyPrintf(...) DoNothing
#define VerboseMessage(Label) DoNothing
#define VerboseProgressHeader(Label, Length) DoNothing
#define VerboseProgress(Value) DoNothing
#endif

//These macros are essentially just malloc/calloc with
//inlined error checking. Do NOT use them in an if/else
//structure without braces, otherwise the compiler gets AIDS
#define SaferMalloc(Type, Pointer, Count) \
(Type*)malloc((size_t)Count * sizeof(Type));\
if (!Pointer) {\
	printf("malloc of size %zu failed!\n", (size_t)Count * sizeof(Type));\
	exit(EXIT_FAILURE);\
}
#define SaferCalloc(Type, Pointer, Count) \
(Type*)calloc(Count, sizeof(Type));\
if (!Pointer) {\
	printf("calloc of count %llu and size %zu failed!\n", (long long)Count, (size_t)sizeof(Type));\
	exit(EXIT_FAILURE);\
}

#define MinRangeCheck(Value, AbsValue, TypeChar) DoSomething(\
if (Value < AbsValue) {\
	printf("Error!\n%s(%"#TypeChar") cannot be less than %s(%"#TypeChar")!", #Value, Value, #AbsValue, AbsValue);\
	exit(EXIT_FAILURE);\
})
#define MaxRangeCheck(Value, AbsValue, TypeChar) DoSomething(\
if (Value > AbsValue) {\
	printf("Error!\n%s(%"#TypeChar") cannot be greater than %s(%"#TypeChar")!", #Value, Value, #AbsValue, AbsValue);\
	exit(EXIT_FAILURE);\
})
#define MinOverMaxCheck(MinValue, MaxValue, TypeChar) DoSomething(\
if (MinValue > MaxValue) {\
	printf("Error!\n%s(%"#TypeChar") cannot be greater than %s(%"#TypeChar")!", #MinValue, MinValue, #MaxValue, MaxValue);\
	exit(EXIT_FAILURE);\
})

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <Windows.h>
#include "resource.h"
#ifdef _DEBUG
#include <assert.h>
#endif

//Used as a boolean
typedef uint_fast8_t Flag;
//If Niantic implements anything over 65536 CP, I give up
typedef uint_least16_t CPValue;
typedef uint_fast16_t FastCPValue;
//Hopefully there'll never be >4 billion outputs
typedef uint_fast32_t CPComboCount;
//Max level is 89
typedef uint_fast8_t CPMIndexer;
typedef uint_fast8_t LevelIndexer;
//Max pokemon is 964
typedef uint_fast16_t PokemonIndexer;
//Max IV is 15
typedef uint_fast8_t IVIndexer;
//IV percentage
typedef double IVPercent;

#pragma pack(push, 1)
typedef struct PokemonStatsStruct {
	uint32_t BaseHP : 10, BaseDefense : 10, BaseAttack : 10, : 0;
} PokemonStatsStruct;

typedef struct CPComboStruct {
	uint_least32_t HPIV : 5, DefenseIV : 5, AttackIV : 5, Level : 7, Index : 10;
} CPComboStruct;
#pragma pack(pop)

inline void* SaferResourceLoad(int ResourceFile) {
	HRSRC GenericResourceHandle = FindResource(NULL, MAKEINTRESOURCE(ResourceFile), RT_RCDATA);
	if (!GenericResourceHandle) {
		printf("Could not find resource %u!", ResourceFile);
		exit(EXIT_FAILURE);
	}
	HGLOBAL GenericResource = LoadResource(NULL, GenericResourceHandle);
	if (!GenericResource) {
		printf("Could not load resource %u!", ResourceFile);
		exit(EXIT_FAILURE);
	}
	return LockResource(GenericResource);
}

int main(int argc, char* argv[]) {
	//Set the process priority as high as possible since maybe it'll run faster
	HANDLE CalculatorProcess = GetCurrentProcess();
	SetPriorityClass(CalculatorProcess, REALTIME_PRIORITY_CLASS);
	//yee
	Flag OutputMode = 0;
	FastCPValue SpecificCP = 0;
	//These are defined as macros so they can be updated gloablly
	Flag VerboseMode = 0;
#ifdef PrintCrap
	uint_fast8_t VerboseDigits;
	char DenominatorName[6] = { '%', '\0', 'c', '%', 'u', '\0' };
	char NumeratorName[5] = { '\r', '%', '\0', 'u', '\0' };
#endif
	PokemonIndexer MinPokemon = AbsMinPokemon, MaxPokemon = AbsMaxPokemon, PokemonCount;
	LevelIndexer MinLevel = AbsMinLevel, MaxLevel = AbsMaxLevel, LevelCount;
	IVIndexer MinAttack = MinIV, MaxAttack = MaxIV;
	IVIndexer MinDefense = MinIV, MaxDefense = MaxIV;
	IVIndexer MinHP = MinIV, MaxHP = MaxIV;
	IVPercent MinPercentIV = (double)(0 / 45), MaxPercentIV = (double)(45 / 45);
	{
		FILE* SettingsFile = fopen(".\\DefaultSettings.ini", "rb");
		if (!SettingsFile) {
			SettingsFile = fopen(".\\DefaultSettings.ini", "wb");
			(void)fprintf(SettingsFile, "FlagFPLevel=1\r\nMinPokemon=1\r\nMaxPokemon=719\r\nMinLevel=1\r\nMaxLevel=40\r\nMinAttack=0\r\nMaxAttack=15\r\nMinDefense=0\r\nMaxDefense=15\r\nMinHP=0\r\nMaxHP=15\r\nMin%%IV=41/45\r\nMax%%IV=45/45");
			(void)freopen(".\\DefaultSettings.ini", "rb", SettingsFile);
		}
		char SettingsBuffer[BUFSIZ];
		uint16_t FPLevelFlag = 0;
		uint16_t Temp8, Temp82;
		double FPLevel;
		fgets(SettingsBuffer, BUFSIZ, SettingsFile);
		while (!feof(SettingsFile)) {
			if (!strncmp(SettingsBuffer, "Min", 3)) {
				if (sscanf(SettingsBuffer, "MinPokemon=%u", &MinPokemon)) {
					--MinPokemon;
					MinRangeCheck(MinPokemon, AbsMinPokemon, u);
				}
				else if (sscanf(SettingsBuffer, "MinAttack=%hu", &Temp8)) {
					MinAttack = (IVIndexer)Temp8;
					MinRangeCheck(MinAttack, MinIV, u);
				}
				else if (sscanf(SettingsBuffer, "MinDefense=%hu", &Temp8)) {
					MinDefense = (IVIndexer)Temp8;
					MinRangeCheck(MinDefense, MinIV, u);
				}
				else if (sscanf(SettingsBuffer, "MinHP=%hu", &Temp8)) {
					MinHP = (IVIndexer)Temp8;
					MinRangeCheck(MinHP, MinIV, u);
				}
				else if (!FPLevelFlag && sscanf(SettingsBuffer, "MinLevel=%hu", &Temp8)) {
					MinLevel = (LevelIndexer)Temp8;
					MinRangeCheck(MinLevel, AbsMinLevel, u);
				}
				else if (FPLevelFlag && sscanf(SettingsBuffer, "MinLevel=%lf", &FPLevel)) {
					MinLevel = (LevelIndexer)((FPLevel * 2) - 1);
					--MinLevel;
					MinRangeCheck(MinLevel, AbsMinLevel, u);
				}
				else if (sscanf(SettingsBuffer, "Min%%IV=%hu/%hu", &Temp8, &Temp82)) {
					MinPercentIV = (double)Temp8 / (double)Temp82;
					MinRangeCheck(MinPercentIV, AbsMinPercentIV, lf);
				}
			}
			else if (!strncmp(SettingsBuffer, "Max", 3)) {
				if (sscanf(SettingsBuffer, "MaxPokemon=%u", &MaxPokemon)) {
					--MaxPokemon;
					MaxRangeCheck(MaxPokemon, AbsMaxPokemon, u);
				}
				else if (sscanf(SettingsBuffer, "MaxAttack=%hu", &Temp8)) {
					MaxAttack = (IVIndexer)Temp8;
					MaxRangeCheck(MaxAttack, MaxIV, u);
				}
				else if (sscanf(SettingsBuffer, "MaxDefense=%hu", &Temp8)) {
					MaxDefense = (IVIndexer)Temp8;
					MaxRangeCheck(MaxDefense, MaxIV, u);
				}
				else if (sscanf(SettingsBuffer, "MaxHP=%hu", &Temp8)) {
					MaxHP = (IVIndexer)Temp8;
					MaxRangeCheck(MaxHP, MaxIV, u);
				}
				else if (!FPLevelFlag && sscanf(SettingsBuffer, "MaxLevel=%hu", &Temp8)) {
					MaxLevel = (LevelIndexer)Temp8;
					MaxRangeCheck(MaxLevel, AbsMaxLevel, u);
				}
				else if (FPLevelFlag && sscanf(SettingsBuffer, "MaxLevel=%lf", &FPLevel)) {
					MaxLevel = (LevelIndexer)((FPLevel * 2) - 1);
					--MaxLevel;
					MaxRangeCheck(MaxLevel, AbsMaxLevel, u);
				}
				else if (sscanf(SettingsBuffer, "Max%%IV=%hu/%hu", &Temp8, &Temp82)) {
					MaxPercentIV = (double)Temp8 / (double)Temp82;
					MaxRangeCheck(MaxPercentIV, AbsMaxPercentIV, lf);
				}
			}
			else if (!strncmp(SettingsBuffer, "Flag", 4)) {
				if (sscanf(SettingsBuffer, "FlagFPLevel=%hu", &FPLevelFlag));
			}
			fgets(SettingsBuffer, BUFSIZ, SettingsFile);
		}
		(void)fclose(SettingsFile);
		MinOverMaxCheck(MinPokemon, MaxPokemon, u);
		MinOverMaxCheck(MinLevel, MaxLevel, u);
		MinOverMaxCheck(MinAttack, MaxAttack, u);
		MinOverMaxCheck(MinDefense, MaxDefense, u);
		MinOverMaxCheck(MinHP, MaxHP, u);
		MinOverMaxCheck(MinPercentIV, MaxPercentIV, lf);
		PokemonCount = (MaxPokemon - MinPokemon) + 1;
		LevelCount = (MaxLevel - MinLevel + 1);
	}
	if (argc > 1) {
		CrappyPrintf("Pokemon GO CP Combination Calculator\n");
	}
	else {
		printf("Pokemon GO CP Combination Calculator\n"
			   "1. XLSX Mode (Verbose)\n"
			   "2. XLSX Mode\n"
			   "3. Text Mode (Verbose)\n"
			   "4. Text Mode\n"
		       "5. Exit\n"
		       "S");
		Flag UserIsDumb = 0;
		int ModeSelect = 0;
		char Nope;
		do {
			printf("elect a mode: ");
			UserIsDumb = scanf("%1u%1[^\n]", &ModeSelect, &Nope);
			FlushStdIn();
			if (UserIsDumb != 1) {
				printf("*Properly* s");
			}
		} while (UserIsDumb != 1);
		switch (ModeSelect) {
			case 1:
				Verbose = 1;
			case 2:
				OutputMode = 0;
				break;
			case 3:
				Verbose = 1;
			case 4:
				OutputMode = 1;
				printf("S");
				do {
					printf("elect a CP value: ");
					UserIsDumb = scanf("%u%1[^\n]", &SpecificCP, &Nope);
					FlushStdIn();
					if (UserIsDumb != 1) {
						printf("*Properly* s");
					}
				} while (UserIsDumb != 1);
				break;
			case 5:
				printf("Yee");
				exit(EXIT_SUCCESS);
			default:
				printf("That wasn't an option. :P");
				exit(EXIT_SUCCESS);
		}
	}
	double* CPM = SaferMalloc(double, CPM, AbsLevelCount);
	float* CPMFloats = (float*)SaferResourceLoad(PokemonCPMFile);
	CPMIndexer CPMIndex = 0;
	VerboseProgressHeader("Loading CPMs:\n", AbsLevelCount - 1);
	for (LevelIndexer Level = AbsMinLevel; Level <= AbsMaxLevel; ++Level) {
		VerboseProgress(Level);
		if (Level & 1) {
#pragma warning(suppress:26451)
			CPM[Level] = sqrt(CPMFloats[CPMIndex] * CPMFloats[CPMIndex] - CPMFloats[CPMIndex] * CPMFloats[CPMIndex] / 2 + CPMFloats[CPMIndex + 1] * CPMFloats[CPMIndex + 1] / 2);
			++CPMIndex;
		}
		else {
			CPM[Level] = (double)CPMFloats[CPMIndex];
		}
		//Square all the CPM values to make later calculations easier
		CPM[Level] *= CPM[Level];
	}
	CPComboCount* CPColumnHeight = SaferCalloc(CPComboCount, CPColumnHeight, ExcelMaxColumns);
	CPValue* CachedCPs = SaferMalloc(CPValue, CachedCPs, PokemonCount * LevelCount * IVCount * IVCount * IVCount);
	CPComboCount CPIndex = 0;
	//This struct is used for holding many temporary values
	PokemonStatsStruct* PokemonStats = (PokemonStatsStruct*)SaferResourceLoad(PokemonStats2File);
	FastCPValue CP;
	uint_fast16_t AttackStats[IVCount];
	double DefenseStats[IVCount], HPStats[IVCount];
	double AttackDefense;
	VerboseProgressHeader("\nPokemon Count Pass 1:\n", PokemonCount - 1);
	PokemonIndexer Pokemon;
	for (Pokemon = MinPokemon; Pokemon <= MaxPokemon; ++Pokemon) {
		VerboseProgress(Pokemon);
		PokemonStatsStruct PokemonStats2 = PokemonStats[Pokemon];
		for (IVIndexer Attack = MinAttack; Attack <= MaxAttack; ++Attack) {
			AttackStats[Attack] = PokemonStats2.BaseAttack + Attack;
		}
		for (IVIndexer Defense = MinDefense; Defense <= MaxDefense; ++Defense) {
#pragma warning(suppress:26451)
			DefenseStats[Defense] = sqrt(PokemonStats2.BaseDefense + Defense);
		}
		for (IVIndexer HP = MinHP; HP <= MaxHP; ++HP) {
#pragma warning(suppress:26451)
			HPStats[HP] = sqrt(PokemonStats2.BaseHP + HP);
		}
		for (LevelIndexer Level = MinLevel; Level <= MaxLevel; ++Level) {
			for (IVIndexer Attack = MinAttack; Attack <= MaxAttack; ++Attack) {
				for (IVIndexer Defense = MinDefense; Defense <= MaxDefense; ++Defense) {
					AttackDefense = AttackStats[Attack] * DefenseStats[Defense];
					for (IVIndexer HP = MinHP; HP <= MaxHP; ++HP) {
						//This counts how much data is going to be in each column
						//of the final output sheet and stores the CP values to a list.
						//This list could also be used for generating sharedstrings
#pragma warning(suppress:26451)
						CP = (FastCPValue)(((AttackDefense * HPStats[HP] * CPM[Level]) / 10) - 1);
						CachedCPs[CPIndex] = CP;
						++CPColumnHeight[CP];
						++CPIndex;
					}
				}
			}
		}
	}
	FastCPValue MinCP = UINT_FAST16_MAX, MaxCP = 0, CPCount;
	for (CP = 0; CP < ExcelMaxColumns; ++CP) {
		if (CPColumnHeight[CP]) {
			if (CP < MinCP) {
				MinCP = CP;
			}
			if (CP > MaxCP) {
				MaxCP = CP;
			}
		}
	}
	CPComboCount MaxRow = 0;
	CPCount = MaxCP + 1;
	CPComboStruct** CPColumn = SaferMalloc(CPComboStruct*, CPColumn, CPCount);
	for (CP = MinCP; CP <= MaxCP; ++CP) {
		if (CPColumnHeight[CP]) {
			//This uses the previously calculated column counts to
			//allocate enough space for the output data, thus avoiding
			//linked lists and the associated RAM usage of their pointers
			if (CPColumnHeight[CP] > MaxRow) {
				MaxRow = CPColumnHeight[CP];
			}
			CPColumn[CP] = SaferMalloc(CPComboStruct, CPColumn[CP], CPColumnHeight[CP]);
			CPColumnHeight[CP] = 0;
		}
	}
	VerboseProgressHeader("\nPokemon Count Pass 2:\n", PokemonCount - 1);
	CPIndex = 0;
	CPComboStruct Combo;
	for (Combo.Index = MinPokemon; Combo.Index <= MaxPokemon; ++Combo.Index) {
		VerboseProgress(Combo.Index);
		for (Combo.Level = MinLevel; Combo.Level <= MaxLevel; ++Combo.Level) {
			for (Combo.AttackIV = MinAttack; Combo.AttackIV <= MaxAttack; ++Combo.AttackIV) {
				for (Combo.DefenseIV = MinDefense; Combo.DefenseIV <= MaxDefense; ++Combo.DefenseIV) {
					for (Combo.HPIV = MinHP; Combo.HPIV <= MaxHP; ++Combo.HPIV) {
						//This (ab)uses the fact that the combinations are looped in the same order
						//as the previous loop, so the CP values can be reused instead of recalculated
						CP = CachedCPs[CPIndex];
						CPColumn[CP][CPColumnHeight[CP]] = Combo;
						++CPColumnHeight[CP];
						++CPIndex;
					}
				}
			}
		}
	}
	free(CachedCPs);
	//Load a file with a bunch of strings representing Excel's column headers
	//Because screw trying to actually calcuate those dang letters on the fly
	//All indices are multiplied by 4 to account for max string length
	if (OutputMode == 0) {
		char* EndMySuffering2 = (char*)SaferResourceLoad(FMLFile);
		char* EndMySuffering = SaferMalloc(char, EndMySuffering, (size_t)CPCount * 4);
		memcpy(EndMySuffering, EndMySuffering2, (size_t)CPCount * 4);
		//Start printing the actual XML
		//"F:\\My Programming Stuff Expansion\\ExcelFileTest\\xl\\worksheets\\sheet1.xml"
		FILE* GenericFile = fopen(".\\sheet1.xml", "w+b");
		(void)fprintf(GenericFile, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" mc:Ignorable=\"x14ac\" xmlns:x14ac=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac\"><dimension ref=\"%s1:%s%u\"/><sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"/></sheetViews><sheetFormatPr defaultRowHeight=\"15\" x14ac:dyDescent=\"0.25\"/><sheetData>", &EndMySuffering[MinCP * 4], &EndMySuffering[MaxCP * 4], MaxRow);
		CPComboCount Row = 0;
		uint_fast16_t TrueLevel[89] = { 10u, 15u, 20u, 25u, 30u, 35u, 40u, 45u, 50u, 55u, 60u, 65u, 70u, 75u, 80u, 85u, 90u, 95u, 100u, 105u, 110u, 115u, 120u, 125u, 130u, 135u, 140u, 145u, 150u, 155u, 160u, 165u, 170u, 175u, 180u, 185u, 190u, 195u, 200u, 205u, 210u, 215u, 220u, 225u, 230u, 235u, 240u, 245u, 250u, 255u, 260u, 265u, 270u, 275u, 280u, 285u, 290u, 295u, 300u, 305u, 310u, 315u, 320u, 325u, 330u, 335u, 340u, 345u, 350u, 355u, 360u, 365u, 370u, 375u, 380u, 385u, 390u, 395u, 400u, 405u, 410u, 415u, 420u, 425u, 430u, 435u, 440u, 445u, 450u };
		//Rows with data in column 0 can store their data without
		//r="A1" type tags until the first gap
		FastCPValue CPLeft = MinCP, CPRight = MaxCP;
		if (MinCP == 0) {
			VerboseProgressHeader("\nRow Parsing 1:\n", CPColumnHeight[0] - 1);
			do {//While a row has data in column 0...
				VerboseProgress(Row);
				(void)fprintf(GenericFile, "<row r=\"%u\" spans=\"1:%u\">", Row + 1, CPRight);
				while (Row > CPColumnHeight[CPRight]) {//Update the right bound of the data
					--CPRight;
				}
				CP = CPLeft;
				do {//...write cell data for that row...
					(void)fprintf(GenericFile, "<c s=\"%u\"><v>%02u%02u%02u%02u</v></c>", CPColumn[CP][Row].Index, TrueLevel[CPColumn[CP][Row].Level], CPColumn[CP][Row].AttackIV, CPColumn[CP][Row].DefenseIV, CPColumn[CP][Row].HPIV);
					++CP;
				} while (Row < CPColumnHeight[CP]);//..until there's a gap in that data.
				++CP;//There was just a gap, so it's safe to increment again
				while (CP <= CPRight) {//While there's still data in the row...
					if (Row < CPColumnHeight[CP]) {//...print cell data for the columns that aren't blank.
						(void)fprintf(GenericFile, "<c r=\"%s%u\" s=\"%u\"><v>%02u%02u%02u%02u</v></c>", &EndMySuffering[CP * 4], Row + 1, CPColumn[CP][Row].Index, TrueLevel[CPColumn[CP][Row].Level], CPColumn[CP][Row].AttackIV, CPColumn[CP][Row].DefenseIV, CPColumn[CP][Row].HPIV);
					}
					++CP;
				}
				++Row;
				(void)fputs("</row>", GenericFile);
			} while (Row < CPColumnHeight[0]);//Ran out of rows with data in column 0, so now the left bound of each row is changing
		}
#ifdef PrintCrap
		else {
			VerboseMessage("\nSkipping Row Parsing 1...");
		}
#endif
		VerboseProgressHeader("\nRow Parsing 2:\n", MaxRow - 1);
		while (Row < MaxRow) {//While there are still more rows...
			VerboseProgress(Row);
			while (Row > CPColumnHeight[CPRight]) {//Update the right bound of the data
				--CPRight;
			}
			while (Row > CPColumnHeight[CPLeft]) {//Update the left bound of the data
				++CPLeft;
			}
			CP = CPLeft;
			(void)fprintf(GenericFile, "<row r=\"%u\" spans=\"%u:%u\">", Row + 1, CPLeft + 1, CPRight + 1);
			do {//Starting from the left bound...
				if (Row < CPColumnHeight[CP]) {//...print cell data for the columns that aren't blank.
					(void)fprintf(GenericFile, "<c r=\"%s%u\" s=\"%u\"><v>%02u%02u%02u%02u</v></c>", &EndMySuffering[CP * 4], Row + 1, CPColumn[CP][Row].Index, TrueLevel[CPColumn[CP][Row].Level], CPColumn[CP][Row].AttackIV, CPColumn[CP][Row].DefenseIV, CPColumn[CP][Row].HPIV);
				}
				++CP;
			} while (CP <= CPRight);
			++Row;
			(void)fputs("</row>", GenericFile);
		};
		//Print the XML footer
		(void)fputs("</sheetData><pageMargins left=\"0.7\" right=\"0.7\" top=\"0.75\" bottom=\"0.75\" header=\"0.3\" footer=\"0.3\"/></worksheet>", GenericFile);
		(void)fclose(GenericFile);
	}
	else {
		if (SpecificCP < MinCP || SpecificCP > MaxCP) {
			printf("Specified CP is outside the calcualted range!");
			exit(EXIT_FAILURE);
		}
		if (!CPColumnHeight[SpecificCP]) {
			printf("No results for specified CP!");
			exit(EXIT_FAILURE);
		}
		char* PokemonNames = (char*)SaferResourceLoad(PokemonNamesFile);
		double TrueLevel[89] = { 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0, 12.5, 13.0, 13.5, 14.0, 14.5, 15.0, 15.5, 16.0, 16.5, 17.0, 17.5, 18.0, 18.5, 19.0, 19.5, 20.0, 20.5, 21.0, 21.5, 22.0, 22.5, 23.0, 23.5, 24.0, 24.5, 25.0, 25.5, 26.0, 26.5, 27.0, 27.5, 28.0, 28.5, 29.0, 29.5, 30.0, 30.5, 31.0, 31.5, 32.0, 32.5, 33.0, 33.5, 34.0, 34.5, 35.0, 35.5, 36.0, 36.5, 37.0, 37.5, 38.0, 38.5, 39.0, 39.5, 40.0, 40.5, 41.0, 41.5, 42.0, 42.5, 43.0, 43.5, 44.0, 44.5, 45.0 };
		FILE* GenericFile = fopen(".\\CPOutput.txt", "w+b");
		VerboseProgressHeader("\nRow Parsing 1:\n", CPColumnHeight[SpecificCP] - 1);
		CPComboStruct TempStruct;
		IVPercent TempPercent;
		for (CPComboCount Row = 0; Row < CPColumnHeight[SpecificCP]; ++Row) {
			VerboseProgress(Row);
			TempStruct = CPColumn[SpecificCP][Row];
			TempPercent = ((double)TempStruct.AttackIV + (double)TempStruct.DefenseIV + (double)TempStruct.HPIV) / (double)(MaxIV + MaxIV + MaxIV);
			if (TempPercent >= MinPercentIV && TempPercent <= MaxPercentIV) {
				(void)fprintf(GenericFile, "%23s/%4.1f/%02u/%02u/%02u\n", &PokemonNames[TempStruct.Index * 24], TrueLevel[TempStruct.Level], TempStruct.AttackIV, TempStruct.DefenseIV, TempStruct.HPIV);
			}
		}
		(void)fclose(GenericFile);
	}
	for (CP = 0; CP <= MaxCP; ++CP) {
		if (CPColumnHeight[CP]) {
			free(CPColumn[CP]);
		}
	}
	free(CPColumn);
	VerboseMessage("\n");
	printf("Done");
	exit(EXIT_SUCCESS);
}
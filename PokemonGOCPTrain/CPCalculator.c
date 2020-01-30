#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MinPokemon 0
#define MaxPokemon 963
#define PokemonCount ((MaxPokemon - MinPokemon) + 1)
#define MinLevel 0
#define MaxLevel 88
#define LevelCount ((MaxLevel - MinLevel) + 1)
#define MinIV 0
#define MaxIV 15
#define IVCount ((MaxIV - MinIV) + 1)
#define ExcelMaxColumns 16384
#define MinCPM 0
#define MaxCPM 44
#define CPMCount ((MaxCPM - MinCPM) + 1)

#define DoSomething(OtherMacro) do {OtherMacro} while (0)
#define DoNothing do {} while (0)

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
//#define SaferMallocOld(Type, Pointer, Size) \
//(Type*)malloc(Size);\
//if (!Pointer) {\
//	printf("malloc of size %zu failed!\n", (size_t)Size);\
//	exit(EXIT_FAILURE);\
//}
#define SaferMalloc(Type, Pointer, Count) \
(Type*)malloc(Count * sizeof(Type));\
if (!Pointer) {\
	printf("malloc of size %zu failed!\n", (size_t)(Count * sizeof(Type)));\
	exit(EXIT_FAILURE);\
}
//#define SaferCallocOld(Type, Pointer, Count, Size) \
//(Type*)calloc(Count, Size);\
//if (!Pointer) {\
//	printf("calloc of count %llu and size %zu failed!\n", (long long)Count, (size_t)Size);\
//	exit(EXIT_FAILURE);\
//}
#define SaferCalloc(Type, Pointer, Count) \
(Type*)calloc(Count, sizeof(Type));\
if (!Pointer) {\
	printf("calloc of count %llu and size %zu failed!\n", (long long)Count, (size_t)sizeof(Type));\
	exit(EXIT_FAILURE);\
}

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

typedef uint_least16_t CPValue;
typedef uint_fast16_t FastCPValue;

#pragma pack(push, 1)
typedef struct PokemonStatsStruct {
	uint32_t BaseHP : 10, BaseDefense : 10, BaseAttack : 10, : 0;
	//uint16_t Form : 3, Dex : 10, : 3;
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

//inline char* VerbosePrintfProgressDenominator(void* Number) {
//	double Intermediate = *(double*)Number;
//	char VerboseBuilder[6] = { '%', (char)(floor(log10(Intermediate) + 1)), 'c', '/', '%', 'u' };
//	return VerboseBuilder;
//}
//
//inline char* VerbosePrintfProgressNumerator(void* Number) {
//	double Intermediate = *(double*)Number;
//	char VerboseBuilder[4] = { '\r', '%', (char)(floor(log10(Intermediate) + 1)), 'u'};
//	return VerboseBuilder;
//}

int main(int argc, char* argv[]) {
	//Set the process priority as high as possible since maybe it'll run faster
	HANDLE CalculatorProcess = GetCurrentProcess();
	SetPriorityClass(CalculatorProcess, REALTIME_PRIORITY_CLASS);
	//uint_fast8_t MinLevel = 0, MaxLevel = 88;
	uint_fast8_t OutputMode = 0;
	//These are defined as macros so they can be updated gloablly
	uint_fast8_t VerboseMode = 0;
	uint_fast8_t VerboseDigits;
	char DenominatorName[6] = { '%', '\0', 'c', '%', 'u', '\0' };
	char NumeratorName[5] = { '\r', '%', '\0', 'u', '\0' };
	uint_fast16_t SpecificCP = 0;
	if (argc > 1) {
		/*uint_fast8_t LastCrapWasFlag;
		for (int i = 1; i < argc; ++i) {
			
		}*/
		CrappyPrintf("Pokemon GO CP Combination Calculator\n");
	}
	else {
		printf("Pokemon GO CP Combination Calculator\n"
			   "1. XLSX Mode (Verbose)\n"
			   "2. XLSX Mode\n"
			   "3. Text Mode (Verbose)\n"
			   "4. Text Mode\n"
		       "5. Exit\n"
		       "Select a mode: ");
		uint_fast8_t UserIsDumb = 0;
		switch (getchar()) {
			case '1':
				Verbose = 1;
			case '2':
				OutputMode = 0;
				break;
			case '3':
				Verbose = 1;
			case '4':
				OutputMode = 1;
				printf("Select a CP value: ");
				do {
					UserIsDumb = scanf("%u", &SpecificCP);
				} while (!UserIsDumb);
				break;
			case '5':
				printf("Yee");
				exit(EXIT_SUCCESS);
			default:
				printf("That wasn't an option. :P");
				exit(EXIT_SUCCESS);
		}
	}
	double* CPM = SaferMalloc(double, CPM, LevelCount);
	float* CPMFloats = (float*)SaferResourceLoad(PokemonCPMFile);
	uint_fast8_t CPMIndex = 0;;
	//CrappyPrintf("Loading CPMs:\n  /%2u\r", LevelCount - 1);
	VerboseProgressHeader("Loading CPMs:\n", LevelCount - 1);
	for (uint_fast8_t Level = MinLevel; Level <= MaxLevel; ++Level) {
		//CrappyPrintf("%2u\r", Level);
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
	uint_least32_t* CPColumnHeight = SaferCalloc(uint_least32_t, CPColumnHeight, ExcelMaxColumns);
	CPValue* CachedCPs = SaferMalloc(CPValue, CachedCPs, PokemonCount * LevelCount * IVCount * IVCount * IVCount);
	uint_fast32_t CPIndex = 0;
	//This struct is used for holding many temporary values
	PokemonStatsStruct* PokemonStats = (PokemonStatsStruct*)SaferResourceLoad(PokemonStats2File);
	FastCPValue CP;
	uint_fast16_t AttackStats[IVCount];
	double DefenseStats[IVCount], HPStats[IVCount];
	double AttackDefense;
	//CrappyPrintf("\nPokemon Count Pass 1:\n   /%3u\r", PokemonCount - 1);
	VerboseProgressHeader("\nPokemon Count Pass 1:\n", PokemonCount - 1);
	uint_fast16_t Pokemon;
	for (Pokemon = MinPokemon; Pokemon <= MaxPokemon; ++Pokemon) {
		//CrappyPrintf("%3u\r", Pokemon);
		VerboseProgress(Pokemon);
		PokemonStatsStruct PokemonStats2 = PokemonStats[Pokemon];
		for (uint_fast8_t IV = MinIV; IV <= MaxIV; ++IV) {
			AttackStats[IV] = PokemonStats2.BaseAttack + IV;
#pragma warning(suppress:26451)
			DefenseStats[IV] = sqrt(PokemonStats2.BaseDefense + IV);
#pragma warning(suppress:26451)
			HPStats[IV] = sqrt(PokemonStats2.BaseHP + IV);
		}
		for (uint_fast8_t Level = MinLevel; Level <= MaxLevel; ++Level) {
			for (uint_fast8_t Attack = MinIV; Attack <= MaxIV; ++Attack) {
				for (uint_fast8_t Defense = MinIV; Defense <= MaxIV; ++Defense) {
					AttackDefense = AttackStats[Attack] * DefenseStats[Defense];
					for (uint_fast8_t HP = MinIV; HP <= MaxIV; ++HP) {
#pragma warning(suppress:26451)
						//This counts how much data is going to be in each column
						//of the final output sheet and stores the CP values to a list.
						//This list could also be used for generating sharedstrings
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
	uint_fast32_t MaxRow = 0;
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
	//CrappyPrintf("\nPokemon Count Pass 2:\n   /%3u\r", PokemonCount - 1);
	VerboseProgressHeader("\nPokemon Count Pass 2:\n", PokemonCount - 1);
	CPIndex = 0;
	CPComboStruct Combo;
	for (Combo.Index = MinPokemon; Combo.Index <= MaxPokemon; ++Combo.Index) {
		//CrappyPrintf("%3u\r", Combo.Index);
		VerboseProgress(Combo.Index);
		for (Combo.Level = MinLevel; Combo.Level <= MaxLevel; ++Combo.Level) {
			for (Combo.AttackIV = MinIV; Combo.AttackIV <= MaxIV; ++Combo.AttackIV) {
				for (Combo.DefenseIV = MinIV; Combo.DefenseIV <= MaxIV; ++Combo.DefenseIV) {
					for (Combo.HPIV = MinIV; Combo.HPIV <= MaxIV; ++Combo.HPIV) {
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
		uint_fast32_t Row = 0;
		uint_fast16_t TrueLevel[89] = { 10u, 15u, 20u, 25u, 30u, 35u, 40u, 45u, 50u, 55u, 60u, 65u, 70u, 75u, 80u, 85u, 90u, 95u, 100u, 105u, 110u, 115u, 120u, 125u, 130u, 135u, 140u, 145u, 150u, 155u, 160u, 165u, 170u, 175u, 180u, 185u, 190u, 195u, 200u, 205u, 210u, 215u, 220u, 225u, 230u, 235u, 240u, 245u, 250u, 255u, 260u, 265u, 270u, 275u, 280u, 285u, 290u, 295u, 300u, 305u, 310u, 315u, 320u, 325u, 330u, 335u, 340u, 345u, 350u, 355u, 360u, 365u, 370u, 375u, 380u, 385u, 390u, 395u, 400u, 405u, 410u, 415u, 420u, 425u, 430u, 435u, 440u, 445u, 450u };
		//Rows with data in column 0 can store their data without
		//r="A1" type tags until the first gap
		//MaxRow = 2;
		//while (Row < 2) {//While a row has data in column 0...
		FastCPValue CPLeft = MinCP, CPRight = MaxCP;
		if (MinCP == 0) {
			//CrappyPrintf("\nRow Parsing 1:\n%u\n", CPColumnHeight[0] - 1);
			VerboseProgressHeader("\nRow Parsing 1:\n", CPColumnHeight[0] - 1);
			do {//While a row has data in column 0...
				//CrappyPrintf("%u\r", Row);
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
			//CrappyPrintf("\nSkipping Row Parsing 1...");
			VerboseMessage("\nSkipping Row Parsing 1...");
		}
#endif
		//CrappyPrintf("\nRow Parsing 2:\n%u\n", MaxRow - 1);
		VerboseProgressHeader("\nRow Parsing 2:\n", MaxRow - 1);
		while (Row < MaxRow) {//While there are still more rows...
			//CrappyPrintf("%u\r", Row);
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
		//CrappyPrintf("\nRow Parsing 1:\n%u\n", CPColumnHeight[SpecificCP] - 1);
		VerboseProgressHeader("\nRow Parsing 1:\n", CPColumnHeight[SpecificCP] - 1);
		for (uint_fast32_t Row = 0; Row < CPColumnHeight[SpecificCP]; ++Row) {
			//CrappyPrintf("%u\r", Row);
			VerboseProgress(Row);
			(void)fprintf(GenericFile, "%23s/%4.1f/%02u/%02u/%02u\n", &PokemonNames[CPColumn[SpecificCP][Row].Index * 24], TrueLevel[CPColumn[SpecificCP][Row].Level], CPColumn[SpecificCP][Row].AttackIV, CPColumn[SpecificCP][Row].DefenseIV, CPColumn[SpecificCP][Row].HPIV);
		}
		//CrappyPrintf("\nSkipping Row Parsing 2...");
		VerboseMessage("\nSkipping Row Parsing 2...");
		(void)fclose(GenericFile);
	}
	for (CP = 0; CP <= MaxCP; ++CP) {
		free(CPColumn[CP]);
	}
	free(CPColumn);
	//CrappyPrintf("\nDone");
	//VerboseMessage("\nDone");
	VerboseMessage("\n");
	printf("Done");
	exit(EXIT_SUCCESS);
}
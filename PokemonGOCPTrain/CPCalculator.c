#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MinPokemon 0
#define MaxPokemon 963
#define PokemonCount (MaxPokemon - MinPokemon) + 1
#define AbsMinLevel 0
#define AbsMaxLevel 88
#define AbsMinLevel2 1
#define AbsMaxLevel2 45
#define LevelCount (MaxLevel - MinLevel) + 1
#define MinIV 0
#define MaxIV 15
#define IVCount (MaxIV - MinIV) + 1
#define ExcelMaxColumns 16384
#define MinCPM 0
#define MaxCPM 44
#define CPMCount (MaxCPM - MinCPM) + 1

//#define PrintCrap

#define DoWrap(OtherMacro) \
do {\
	OtherMacro\
} while (0)

//This macro is literally just a toggleable printf.
//The syntax is the same as the regular printf, but
//it can be toggled on/off by defining PrintCrap
#ifdef PrintCrap
#define CrappyPrintf(...) DoWrap(printf(__VA_ARGS__);)
#else
#define CrappyPrintf(...) do {} while (0)
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
//#include <Windows.h>
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
	//uint32_t HPIV : 4, DefenseIV : 4, AttackIV : 4, Level : 7, Form : 3, Dex : 10;
	uint_least32_t HPIV : 5, DefenseIV : 5, AttackIV : 5, Level : 7, Index : 10;
} CPComboStruct;
#pragma pack(pop)

int main(int argc, char* argv[]) {
	//Set the process priority as high as possible since maybe it'll run faster
	//HANDLE CalculatorProcess = GetCurrentProcess();
	//SetPriorityClass(CalculatorProcess, REALTIME_PRIORITY_CLASS);
	uint_fast8_t MinLevel, MaxLevel;
	if (argc > 1) {
		uint_fast8_t LastCrapWasFlag;
		for (int i = 1; i < argc; ++i) {
			
		}
	}
	else {
		MinLevel = 0;
		MaxLevel = 88;
	}
	CrappyPrintf("Pokemon GO CP Combination Calculator\nLoading CPMs\n%u\n", LevelCount);
	FILE* GenericFile = fopen(".\\PokemonCPM.bin", "rb");
	double* CPM = SaferMalloc(double, CPM, LevelCount);
	float* CPMFloats = SaferMalloc(float, CPMFloats, CPMCount);
	(void)fread(CPMFloats, CPMCount, sizeof(float), GenericFile);
	uint_fast8_t CPMIndex = 0;
	for (uint_fast8_t Level = MinLevel; Level <= MaxLevel; ++Level) {
		CrappyPrintf("%u\r", Level);
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
	free(CPMFloats);
	uint_least32_t* CPColumnHeight = SaferCalloc(uint32_t, CPColumnHeight, ExcelMaxColumns);
	CPValue* CachedCPs = SaferMalloc(CPValue, CachedCPs, PokemonCount * LevelCount * IVCount * IVCount * IVCount);
	uint_fast32_t CPIndex = 0;
	//This struct is used for holding many temporary values
	(void)freopen(".\\PokemonStats2.bin", "rb", GenericFile);
	PokemonStatsStruct PokemonStats2;
	FastCPValue CP;
	uint_fast16_t AttackStats[IVCount];
	double DefenseStats[IVCount], HPStats[IVCount];
	double AttackDefense;
	CrappyPrintf("\nPokemon Count Pass 1:\n%u\n", PokemonCount);
	//uint_fast8_t Attack, Defense, HP;
	uint_fast16_t Pokemon;
	for (Pokemon = MinPokemon; Pokemon <= MaxPokemon; ++Pokemon) {
		CrappyPrintf("%u\r", Pokemon);
		(void)fread(&PokemonStats2, sizeof(PokemonStatsStruct), 1, GenericFile);
		for (uint_fast8_t IV = MinIV; IV <= MaxIV; ++IV) {
			AttackStats[IV] = PokemonStats2.BaseAttack + IV;
#pragma warning(suppress:26451)
			DefenseStats[IV] = sqrt(PokemonStats2.BaseDefense + IV);
#pragma warning(suppress:26451)
			HPStats[IV] = sqrt(PokemonStats2.BaseHP + IV);
		}
		for (uint_fast8_t Level = MinLevel; Level <= MaxLevel; ++Level) {
		//for (Combo.Level = MinLevel; Combo.Level <= MaxLevel; ++Combo.Level) {
			for (uint_fast8_t Attack = MinIV; Attack <= MaxIV; ++Attack) {
				for (uint_fast8_t Defense = MinIV; Defense <= MaxIV; ++Defense) {
					AttackDefense = AttackStats[Attack] * DefenseStats[Defense];
					for (uint_fast8_t HP = MinIV; HP <= MaxIV; ++HP) {
						//#pragma warning(suppress:6011)
						//#pragma warning(suppress:6385)
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
	CrappyPrintf("\nPokemon Count Pass 2:\n%u\n", PokemonCount);
	CPIndex = 0;
	CPComboStruct Combo;
	for (Combo.Index = MinPokemon; Combo.Index <= MaxPokemon; ++Combo.Index) {
		CrappyPrintf("%u\r", Combo.Index);
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
	char* EndMySuffering = SaferMalloc(char, EndMySuffering, (size_t)CPCount * 4);
	(void)freopen(".\\FML.bin", "rb", GenericFile);
	(void)fread(EndMySuffering, (size_t)CPCount * 4, sizeof(char), GenericFile);
	//Start printing the actual XML
	(void)freopen("F:\\My Programming Stuff Expansion\\ExcelFileTest\\xl\\worksheets\\sheet1.xml", "w+b", GenericFile);
	(void)fprintf(GenericFile, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" mc:Ignorable=\"x14ac\" xmlns:x14ac=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac\"><dimension ref=\"%s1:%s%u\"/><sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"/></sheetViews><sheetFormatPr defaultRowHeight=\"15\" x14ac:dyDescent=\"0.25\"/><sheetData>", &EndMySuffering[MinCP * 4], &EndMySuffering[MaxCP * 4], MaxRow);
	uint_fast32_t Row = 0;
	uint_fast16_t TrueLevel[89] = { 10u, 15u, 20u, 25u, 30u, 35u, 40u, 45u, 50u, 55u, 60u, 65u, 70u, 75u, 80u, 85u, 90u, 95u, 100u, 105u, 110u, 115u, 120u, 125u, 130u, 135u, 140u, 145u, 150u, 155u, 160u, 165u, 170u, 175u, 180u, 185u, 190u, 195u, 200u, 205u, 210u, 215u, 220u, 225u, 230u, 235u, 240u, 245u, 250u, 255u, 260u, 265u, 270u, 275u, 280u, 285u, 290u, 295u, 300u, 305u, 310u, 315u, 320u, 325u, 330u, 335u, 340u, 345u, 350u, 355u, 360u, 365u, 370u, 375u, 380u, 385u, 390u, 395u, 400u, 405u, 410u, 415u, 420u, 425u, 430u, 435u, 440u, 445u, 450u };
	//Rows with data in column 0 can store their data without
	//r="A1" type tags until the first gap
	//MaxRow = 2;
	//while (Row < 2) {//While a row has data in column 0...
	FastCPValue CPLeft = MinCP, CPRight = MaxCP;
	if (MinCP == 0) {
		CrappyPrintf("\nRow Parsing 2:\n%u\n", CPColumnHeight[0]);
		do {//While a row has data in column 0...
			CrappyPrintf("%u\r", Row);
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
		CrappyPrintf("\nSkipping Row Parsing 2...\n");
	}
#endif
	CrappyPrintf("\nRow Parsing 3:\n%u\n", MaxRow - 1);
	while (Row < MaxRow) {//While there are still more rows...
		CrappyPrintf("%u\r", Row);
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
	free(EndMySuffering);
	//Print the XML footer
	(void)fputs("</sheetData><pageMargins left=\"0.7\" right=\"0.7\" top=\"0.75\" bottom=\"0.75\" header=\"0.3\" footer=\"0.3\"/></worksheet>", GenericFile);
	(void)fclose(GenericFile);
	for (CP = 0; CP <= MaxCP; ++CP) {
		free(CPColumn[CP]);
	}
	free(CPColumn);
	printf("yeet");
	CrappyPrintf("\nDone");
	exit(EXIT_SUCCESS);
}
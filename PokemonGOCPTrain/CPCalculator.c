#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MinPokemon 0
#define MaxPokemon 963
#define PokemonCount 964
#define MinLevel 0
#define MaxLevel 88
#define LevelCount 89
#define MinIV 0
#define MaxIV 15
#define IVCount 16
//#define MinCP 0
//#define MaxCP 10402
//#define CPCount 10403
#define ExcelMaxColumns 16384
#define MinCPM 0
#define MaxCPM 44
#define CPMCount 45

#define PrintCrap

#define DoWrap(OtherMacro) \
do {\
	OtherMacro\
} while (0)

//This macro is literally just a toggleable printf.
//The syntax is the same as the regular printf, but
//it can be toggled on/off by defining PrintCrap
#ifdef PrintCrap
#define CrappyPrintf(...) DoWrap((void)printf(__VA_ARGS__);)
#else
#define CrappyPrintf(...) DoWrap(/*yeet*/)
#endif

//These macros are essentially just malloc/calloc with
//inlined error checking. Do NOT use them in an if/else
//structure without braces, otherwise the compiler gets AIDS
#define SaferMalloc(Type, Pointer, Size) \
(Type*)malloc(Size);\
if (!Pointer) {\
	printf("malloc of size %zu failed!\n", (size_t)Size);\
	exit(1);\
}
#define SaferMalloc2(Type, Pointer, Count) \
(Type*)malloc(Count * sizeof(Type));\
if (!Pointer) {\
	printf("malloc of size %zu failed!\n", (size_t)(Count * sizeof(Type)));\
	exit(1);\
}
#define SaferCalloc(Type, Pointer, Count, Size) \
(Type*)calloc(Count, Size);\
if (!Pointer) {\
	printf("calloc of count %llu and size %zu failed!\n", (long long)Count, (size_t)Size);\
	exit(1);\
}
#define SaferCalloc2(Type, Pointer, Count) \
(Type*)calloc(Count, sizeof(Type));\
if (!Pointer) {\
	printf("calloc of count %llu and size %zu failed!\n", (long long)Count, (size_t)sizeof(Type));\
	exit(1);\
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <Windows.h>
#ifdef _DEBUG
#include <assert.h>
#endif

typedef uint16_t CPValue;
typedef uint_fast16_t FastCPValue;

#pragma pack(push, 1)
typedef struct PokemonStatsStruct {
	uint32_t BaseHP : 10, BaseDefense : 10, BaseAttack : 10, : 0;
	//uint16_t Form : 3, Dex : 10, : 3;
} PokemonStatsStruct;

typedef struct CPComboStruct {
	//uint32_t HPIV : 4, DefenseIV : 4, AttackIV : 4, Level : 7, Form : 3, Dex : 10;
	uint32_t HPIV : 5, DefenseIV : 5, AttackIV : 5, Level : 7, Index : 10;
} CPComboStruct;
#pragma pack(pop)

void main() {
	//Set the process priority as high as possible since maybe it'll run faster
	HANDLE CalculatorProcess = GetCurrentProcess();
	SetPriorityClass(CalculatorProcess, REALTIME_PRIORITY_CLASS);
	CrappyPrintf("Pokemon GO CP Combination Calculator\nLoading CPMs\n%u\n", LevelCount);
	uint32_t* CPColumnHeight = SaferCalloc2(uint32_t, CPColumnHeight, ExcelMaxColumns);
	CPValue* CachedCPs = SaferMalloc2(CPValue, CachedCPs, PokemonCount * LevelCount * IVCount * IVCount * IVCount);
	uint_fast32_t CPIndex = 0;
	//This struct is used for holding many temporary values
	CPComboStruct Combo;
	FILE* CPMFile = fopen(".\\PokemonCPM.bin", "rb");
	float CPMF[2];
	(void)fread(&CPMF[0], sizeof(float), 1, CPMFile);
	double CPMD;
	FILE* StatsFile = fopen(".\\PokemonStats2.bin", "rb");
	PokemonStatsStruct PokemonStats2;
	FastCPValue CP;
	double CP1, CP2;
	CrappyPrintf("\nPokemon Count Pass 1:\n%u\n", PokemonCount);
	uint_fast16_t Attack, Defense, HP;
	uint_fast8_t IV;
	uint_fast16_t AttackStats[IVCount];
	double DefenseStats[IVCount], HPStats[IVCount];
	double DefenseRoot, AttackDefense;
	double CPMDAttack, CPMDAttackDefense;
	for (uint_fast16_t Pokemon = MinPokemon; Pokemon <= MinPokemon; ++Pokemon) {
		CrappyPrintf("%u\r", Pokemon);
		(void)fread(&PokemonStats2, sizeof(PokemonStatsStruct), 1, StatsFile);
		for (IV = MinIV; IV <= MaxIV; ++IV) {
			AttackStats[IV] = PokemonStats2.BaseAttack + IV;
			DefenseStats[IV] = sqrt(PokemonStats2.BaseDefense + IV);
			HPStats[IV] = sqrt(PokemonStats2.BaseHP + IV);
		}
		for (Combo.Level = MinLevel; Combo.Level <= MaxLevel; ++Combo.Level) {
			if (Combo.Level & 1) {
				(void)fread(&CPMF[1], sizeof(float), 1, CPMFile);
#pragma warning(suppress:26451)
				CPMD = sqrt(CPMF[0] * CPMF[0] - CPMF[0] * CPMF[0] / 2 + CPMF[1] * CPMF[1] / 2);
				CPMF[0] = CPMF[1];
			}
			else {
				CPMD = (double)CPMF[0];
			}
			//Square the CPM value to make later calculations easier
			CPMD *= CPMD;
			Combo.AttackIV = 0;
			for (Attack = MinIV; Attack <= MaxIV; ++Attack) {
				//Attack = PokemonStats2.BaseAttack + Combo.AttackIV;
				Combo.DefenseIV = 0;
				for (Defense = MinIV; Defense <= MaxIV; ++Defense) {
					//Defense = PokemonStats2.BaseDefense + Combo.DefenseIV;
					//DefenseRoot = sqrt(PokemonStats2.BaseDefense + Combo.DefenseIV);
					AttackDefense = AttackStats[Attack] * DefenseStats[Defense];
					Combo.HPIV = 0;
					for (HP = MinIV; HP <= MaxIV; ++HP) {
//#pragma warning(suppress:6011)
//#pragma warning(suppress:6385)
#pragma warning(suppress:26451)
						//This counts how much data is going to be in each column
						//of the final output sheet and stores the CP values to a list.
						//This list could also be used for generating sharedstrings
						CP1 = ((PokemonStats2.BaseAttack + Combo.AttackIV) * sqrt(PokemonStats2.BaseDefense + Combo.DefenseIV) * sqrt(PokemonStats2.BaseHP + Combo.HPIV) * CPMD) / 10;
						CP2 = (AttackDefense * HPStats[HP] * CPMD) / 10;
						assert(CP1 == CP2);
						//CP = (CPValue)((((PokemonStats2.BaseAttack + Combo.AttackIV) * sqrt(PokemonStats2.BaseDefense + Combo.DefenseIV) * sqrt(PokemonStats2.BaseHP + Combo.HPIV) * CPMD) / 10) - 1);
						//CachedCPs[CPIndex] = CP;
						//++CPColumnHeight[CP];
						//++CPIndex;
						++Combo.HPIV;
					}
					++Combo.DefenseIV;
				}
				++Combo.AttackIV;
			}
		}
	}
	(void)fclose(CPMFile);
	(void)fclose(StatsFile);
	exit(0);
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
	CPComboStruct** CPColumn = SaferMalloc2(CPComboStruct*, CPColumn, CPCount);
	for (CP = MinCP; CP <= MaxCP; ++CP) {
		if (CPColumnHeight[CP]) {
			//This uses the previously calculated column counts to
			//allocate enough space for the output data, thus avoiding
			//linked lists and the associated RAM usage of their pointers
			if (CPColumnHeight[CP] > MaxRow) {
				MaxRow = CPColumnHeight[CP];
			}
			CPColumn[CP] = SaferMalloc2(CPComboStruct, CPColumn[CP], CPColumnHeight[CP]);
			CPColumnHeight[CP] = 0;
		}
	}
	CrappyPrintf("\nPokemon Count Pass 2:\n%u\n", PokemonCount);
	CPIndex = 0;
	for (Combo.Index = MinPokemon; Combo.Index <= MaxPokemon; ++Combo.Index) {
		CrappyPrintf("%u\r", Combo.Index);
		for (Combo.Level = MinLevel; Combo.Level <= MaxLevel; ++Combo.Level) {
			for (Combo.AttackIV = MinIV; Combo.AttackIV <= MaxIV; ++Combo.AttackIV) {
				for (Combo.DefenseIV = MinIV; Combo.DefenseIV <= MaxIV; ++Combo.DefenseIV) {
					for (Combo.HPIV = MinIV; Combo.HPIV <= MaxIV; ++Combo.HPIV) {
						//This (ab)uses the fact that the combinations are looped in the same order
						//as the previous loop, so the CP values can be reused instead of recalculated
						CPColumn[CachedCPs[CPIndex]][CPColumnHeight[CachedCPs[CPIndex]]] = Combo;
						++CPColumnHeight[CachedCPs[CPIndex]];
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
	//char* EndMySuffering = SaferMalloc(char, EndMySuffering, CPCount * 4);
	char* EndMySuffering = SaferMalloc2(char, EndMySuffering, (size_t)CPCount * 4);
	FILE* AAAAAAAAH = fopen("FML.bin", "rb");
	(void)fread(EndMySuffering, (size_t)CPCount * 4, sizeof(char), AAAAAAAAH);
	(void)fclose(AAAAAAAAH);
	//Start printing the actual XML
	FILE* PokemonOutputFile = fopen("F:\\My Programming Stuff Expansion\\ExcelFileTest\\xl\\worksheets\\sheet1.xml", "w+");
	(void)fprintf(PokemonOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" mc:Ignorable=\"x14ac\" xmlns:x14ac=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac\"><dimension ref=\"%s1:%s%u\"/><sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"/></sheetViews><sheetFormatPr defaultRowHeight=\"15\" x14ac:dyDescent=\"0.25\"/><sheetData>", &EndMySuffering[MinCP * 4], &EndMySuffering[MaxCP * 4], MaxRow);//<cols><col min=\"%u\" max=\"%u\" bestFit=\"1\" customWidth=\"1\"/></cols>
	uint_fast32_t Row = 0;
	uint_fast16_t TrueLevel[90] = { 0u, 10u, 15u, 20u, 25u, 30u, 35u, 40u, 45u, 50u, 55u, 60u, 65u, 70u, 75u, 80u, 85u, 90u, 95u, 100u, 105u, 110u, 115u, 120u, 125u, 130u, 135u, 140u, 145u, 150u, 155u, 160u, 165u, 170u, 175u, 180u, 185u, 190u, 195u, 200u, 205u, 210u, 215u, 220u, 225u, 230u, 235u, 240u, 245u, 250u, 255u, 260u, 265u, 270u, 275u, 280u, 285u, 290u, 295u, 300u, 305u, 310u, 315u, 320u, 325u, 330u, 335u, 340u, 345u, 350u, 355u, 360u, 365u, 370u, 375u, 380u, 385u, 390u, 395u, 400u, 405u, 410u, 415u, 420u, 425u, 430u, 435u, 440u, 445u, 450u };
	//Rows with data in column 0 can store their data without
	//r="A1" type tags until the first gap
	//MaxRow = 2;
	//while (Row < 2) {//While a row has data in column 0...
	FastCPValue CPLeft = MinCP, CPRight = MaxCP;
	if (MinCP == 0) {
		CrappyPrintf("\nRow Parsing 2:\n%u\n", CPColumnHeight[0]);
		do {//While a row has data in column 0...
			CrappyPrintf("%u\r", Row);
			(void)fprintf(PokemonOutputFile, "<row r=\"%u\" spans=\"1:%u\">", Row + 1, CPRight);
			while (Row > CPColumnHeight[CPRight]) {//Update the right bound of the data
				--CPRight;
			}
			CP = CPLeft;
			do {//...write cell data for that row...
				(void)fprintf(PokemonOutputFile, "<c s=\"%u\"><v>%02u%02u%02u%02u</v></c>", CPColumn[CP][Row].Index, TrueLevel[CPColumn[CP][Row].Level], CPColumn[CP][Row].AttackIV, CPColumn[CP][Row].DefenseIV, CPColumn[CP][Row].HPIV);
				++CP;
			} while (Row < CPColumnHeight[CP]);//..until there's a gap in that data.
			++CP;//There was just a gap, so it's safe to increment again
			while (CP <= CPRight) {//While there's still data in the row...
				if (Row < CPColumnHeight[CP]) {//...print cell data for the columns that aren't blank.
					(void)fprintf(PokemonOutputFile, "<c r=\"%s%u\" s=\"%u\"><v>%02u%02u%02u%02u</v></c>", &EndMySuffering[CP * 4], Row + 1, CPColumn[CP][Row].Index, TrueLevel[CPColumn[CP][Row].Level], CPColumn[CP][Row].AttackIV, CPColumn[CP][Row].DefenseIV, CPColumn[CP][Row].HPIV);
				}
				++CP;
			}
			++Row;
			(void)fprintf(PokemonOutputFile, "</row>");
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
		(void)fprintf(PokemonOutputFile, "<row r=\"%u\" spans=\"%u:%u\">", Row + 1, CPLeft + 1, CPRight + 1);
		do {//Starting from the left bound...
			if (Row < CPColumnHeight[CP]) {//...print cell data for the columns that aren't blank.
				(void)fprintf(PokemonOutputFile, "<c r=\"%s%u\" s=\"%u\"><v>%02u%02u%02u%02u</v></c>", &EndMySuffering[CP * 4], Row + 1, CPColumn[CP][Row].Index, TrueLevel[CPColumn[CP][Row].Level], CPColumn[CP][Row].AttackIV, CPColumn[CP][Row].DefenseIV, CPColumn[CP][Row].HPIV);
			}
			++CP;
		} while (CP <= CPRight);
		++Row;
		(void)fprintf(PokemonOutputFile, "</row>");
	};
	free(EndMySuffering);
	//Print the XML footer
	(void)fprintf(PokemonOutputFile, "</sheetData><pageMargins left=\"0.7\" right=\"0.7\" top=\"0.75\" bottom=\"0.75\" header=\"0.3\" footer=\"0.3\"/></worksheet>");
	(void)fclose(PokemonOutputFile);
	for (CP = 0; CP <= MaxCP; ++CP) {
		free(CPColumn[CP]);
	}
	free(CPColumn);
	CrappyPrintf("\nDone");
}
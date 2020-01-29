#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MinPokemon 1
#define MaxPokemon 964
#define MinLevel 1
#define MaxLevel 89
#define MinIV 0
#define MaxIV 15
#define IVCount 16
#define MinCP 1
#define MaxCP 10403
#define MinCPM 1
#define MaxCPM 45

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
#define SaferCalloc(Type, Pointer, Count, Size) \
(Type*)calloc(Count, Size);\
if (!Pointer) {\
	printf("calloc of count %llu and size %zu failed!\n", (long long)Count, (size_t)Size);\
	exit(1);\
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <Windows.h>

#pragma pack(push, 1)
typedef struct PokemonStatsStruct {
	uint32_t BaseHP : 10, BaseDefense : 10, BaseAttack : 10, : 0;
	//uint16_t Form : 3, Dex : 10, : 3;
} PokemonStatsStruct;

typedef struct CPComboStruct {
	//uint32_t HPIV : 4, DefenseIV : 4, AttackIV : 4, Level : 7, Form : 3, Dex : 10;
	uint32_t HPIV : 4, DefenseIV : 4, AttackIV : 4, Level : 7, Index : 10;
} CPComboStruct;

typedef struct CPColumnMetadata {
	uint_fast32_t Height;
	CPComboStruct* Data;
} CPColumnMetadata;
#pragma pack(pop)

void main() {
	//Set the process priority as high as possible since maybe it'll run faster
	HANDLE CalculatorProcess = GetCurrentProcess();
	SetPriorityClass(CalculatorProcess, REALTIME_PRIORITY_CLASS);
	CrappyPrintf("Pokemon GO CP Combination Calculator\nLoading CPMs\n%u\n", MaxLevel - 1);
	//Load CPM values from the data file and calculate the double values used later
	FILE* CPMFile = fopen(".\\PokemonCPM.bin", "rb");
	double* CPM = SaferMalloc(double, CPM, MaxLevel * sizeof(double));
	float* CPMFloats = SaferMalloc(float, CPMFloats, MaxCPM * sizeof(float));
	(void)fread(CPMFloats, MaxCPM, sizeof(float), CPMFile);
	uint_fast8_t CPMIndex = 0;
	for (uint_fast8_t Level = 0; Level < MaxLevel; ++Level) {
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
	(void)fclose(CPMFile);
	free(CPMFloats);
	//Load the pokemon stats from a file
	FILE* StatsFile = fopen(".\\PokemonStats2.bin", "rb");
	PokemonStatsStruct* PokemonStats = SaferMalloc(PokemonStatsStruct, PokemonStats, MaxPokemon * sizeof(PokemonStatsStruct));
	(void)fread(PokemonStats, sizeof(PokemonStatsStruct), MaxPokemon, StatsFile);
	(void)fclose(StatsFile);
	//This line needs to be calloc instead of malloc to initialize Height to 0 without looping the array
	CPColumnMetadata* CPColumn = SaferCalloc(CPColumnMetadata, CPColumn, MaxCP, sizeof(CPColumnMetadata));
	uint16_t* CachedCPs = SaferMalloc(uint16_t, CachedCPs, MaxPokemon * MaxLevel * IVCount * IVCount * IVCount * sizeof(uint16_t));
	uint_fast32_t CPIndex = 0;
	//This struct is used for holding many temporary values
	CPComboStruct Combo = { .AttackIV = 0, .DefenseIV = 0, .HPIV = 0 };
	CrappyPrintf("\nPokemon Count Pass 1:\n%u\n", MaxPokemon - 1);
	for (uint_fast16_t Pokemon = 0; Pokemon < MaxPokemon; ++Pokemon) {
		CrappyPrintf("%u\r", Pokemon);
		for (Combo.Level = MinLevel - 1; Combo.Level < MaxLevel; ++Combo.Level) {
			//I couldn't get this to work in a for loop after
			//several hours or trying. Intentional overflow FTW I guess
			do {
				do {
					do {
#pragma warning(suppress:6011)
#pragma warning(suppress:6385)
#pragma warning(suppress:26451)
						//This counts how much data is going to be in each column
						//of the final output sheet and stores the CP values to a list.
						//This list could also be used for generating sharedstrings
						CachedCPs[CPIndex] = (uint16_t)((((PokemonStats[Pokemon].BaseAttack + Combo.AttackIV) * sqrt(PokemonStats[Pokemon].BaseDefense + Combo.DefenseIV) * sqrt(PokemonStats[Pokemon].BaseHP + Combo.HPIV) * CPM[Combo.Level]) / 10) - 1);
						++CPColumn[CachedCPs[CPIndex]].Height;
						++CPIndex;
					} while (Combo.HPIV++ < MaxIV);
				} while (Combo.DefenseIV++ < MaxIV);
			} while (Combo.AttackIV++ < MaxIV);
		}
	}
	free(PokemonStats);
	uint_fast16_t CP;
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		if (CPColumn[CP].Height) {
			//This uses the previously calculated column counts to
			//allocate enough space for the output data, thus avoiding
			//linked lists and the associated RAM usage of their pointers
			CPColumn[CP].Data = SaferMalloc(CPComboStruct, CPColumn[CP].Data, CPColumn[CP].Height * sizeof(CPComboStruct));
			CPColumn[CP].Height = 0;
		}
	}
	CrappyPrintf("\nPokemon Count Pass 2:\n%u\n", MaxPokemon - 1);
	CPIndex = 0;
	for (Combo.Index = MinPokemon; Combo.Index <= MaxPokemon; ++Combo.Index) {
		CrappyPrintf("%u\r", Combo.Index);
		//Combo.Dex = PokemonStats[Pokemon].Dex;
		//Combo.Form = PokemonStats[Pokemon].Form;
		for (Combo.Level = MinLevel; Combo.Level <= MaxLevel; ++Combo.Level) {
			do {
				do {
					do {
						//This (ab)uses the fact that the combinations are looped in the same order
						//as the previous loop, so the CP values can be reused instead of recalculated
						CPColumn[CachedCPs[CPIndex]].Data[CPColumn[CachedCPs[CPIndex]].Height] = Combo;
						++CPColumn[CachedCPs[CPIndex]].Height;
						++CPIndex;
					} while (Combo.HPIV++ < MaxIV);
				} while (Combo.DefenseIV++ < MaxIV);
			} while (Combo.AttackIV++ < MaxIV);
		}
	}
	free(CachedCPs);
	//CrappyPrintf("\nCP Count:\n%u\n", CPIndex - 1);
	uint_fast32_t MaxRow = 0;
	//Calculate the maximum row the data can go down to
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		if (CPColumn[CP].Height) {
			if (CPColumn[CP].Height > MaxRow) {
				MaxRow = CPColumn[CP].Height;
			}
		}
	}
	//Calculate the Right bound of the data in each row
	uint_fast32_t Row = 0;
	uint16_t* CPRowMax = SaferMalloc(uint16_t, CPRowMax, MaxRow * sizeof(uint16_t));
	CP = MaxCP - 1;
	Row = 0;
	CrappyPrintf("\nRow Parsing 1:\n%u\n", MaxRow - 1);
	do {
		CrappyPrintf("%u\r", Row);
		while (Row < CPColumn[CP].Height) {
			CPRowMax[Row] = CP;
			++Row;
		}
		--CP;
	} while (Row < MaxRow);
	//Load a file with a bunch of strings representing Excel's column headers
	//Because screw trying to actually calcuate those dang letters on the fly
	//All indices are multiplied by 4 to account for max string length
	char* EndMySuffering = SaferMalloc(char, EndMySuffering, MaxCP * 4);
	FILE* AAAAAAAAH = fopen("FML.bin", "rb");
	(void)fread(EndMySuffering, MaxCP * 4, sizeof(char), AAAAAAAAH);
	(void)fclose(AAAAAAAAH);
	//Start printing the actual XML
	FILE* PokemonOutputFile = fopen("F:\\My Programming Stuff Expansion\\ExcelFileTest\\xl\\worksheets\\sheet1.xml", "w+");
	(void)fprintf(PokemonOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" mc:Ignorable=\"x14ac\" xmlns:x14ac=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac\"><dimension ref=\"A1:%s%u\"/><sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"/></sheetViews><sheetFormatPr defaultRowHeight=\"15\" x14ac:dyDescent=\"0.25\"/><sheetData>", &EndMySuffering[MaxCP * 4 - 4], MaxRow);//<cols><col min=\"%u\" max=\"%u\" bestFit=\"1\" customWidth=\"1\"/></cols>
	Row = 0;
	uint_fast8_t TrueLevel[90] = { 0, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 255, 260, 265, 270, 275, 280, 285, 290, 295, 300, 305, 310, 315, 320, 325, 330, 335, 340, 345, 350, 355, 360, 365, 370, 375, 380, 385, 390, 395, 400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 450 };
	CrappyPrintf("\nRow Parsing 2:\n%u\n", CPColumn[0].Height - 1);
	//Rows with data in column 0 can store their data without
	//r="A1" type tags until the first gap
	//MaxRow = 2;
	//CP = MinCP - 1;
	//while (Row < 2) {//While a row has data in column 0...
	while (Row < CPColumn[0].Height) {//While a row has data in column 0...
		CrappyPrintf("%u\r", Row);
		(void)fprintf(PokemonOutputFile, "<row r=\"%u\" spans=\"%u:%u\">", Row + 1, 1, CPRowMax[Row] + 1);
		CP = MinCP - 1;
		do {//...write cell data for that row...
			(void)fprintf(PokemonOutputFile, "<c s=\"%u\"><v>%02u%02u%02u%02u</v></c>", CPColumn[CP].Data[Row].Index, TrueLevel[CPColumn[CP].Data[Row].Level], CPColumn[CP].Data[Row].AttackIV, CPColumn[CP].Data[Row].DefenseIV, CPColumn[CP].Data[Row].HPIV);
			++CP;
		} while (Row < CPColumn[CP].Height);//..until there's a gap in that data.
		while (CP <= CPRowMax[Row]) {//While there's still data in the row...
			if (Row < CPColumn[CP].Height) {//...print cell data for the columns that aren't blank.
				(void)fprintf(PokemonOutputFile, "<c r=\"%s%u\" s=\"%u\"><v>%02u%02u%02u%02u</v></c>", &EndMySuffering[CP * 4], Row + 1, CPColumn[CP].Data[Row].Index, TrueLevel[CPColumn[CP].Data[Row].Level], CPColumn[CP].Data[Row].AttackIV, CPColumn[CP].Data[Row].DefenseIV, CPColumn[CP].Data[Row].HPIV);
			}
			++CP;
		}
		++Row;
		(void)fprintf(PokemonOutputFile, "</row>");
	}//Ran out of rows with data in column 0, so now the left bound of each row is changing
	uint_fast16_t CPLeft = MinCP - 1;
	CrappyPrintf("\nRow Parsing 3:\n%u\n", MaxRow - 1);
	while (Row < MaxRow) {//While there are still more rows...
		CrappyPrintf("%u\r", Row);
		while (Row > CPColumn[CPLeft].Height) {//Update the left bound of the data
			++CPLeft;
		}
		CP = CPLeft;
		(void)fprintf(PokemonOutputFile, "<row r=\"%u\" spans=\"%u:%u\">", Row + 1, CPLeft + 1, CPRowMax[Row] + 1);
		while (CP <= CPRowMax[Row]) {//Starting from the left bound...
			if (Row < CPColumn[CP].Height) {//...print cell data for the columns that aren't blank.
				(void)fprintf(PokemonOutputFile, "<c r=\"%s%u\" s=\"%u\"><v>%02u%02u%02u%02u</v></c>", &EndMySuffering[CP * 4], Row + 1, CPColumn[CP].Data[Row].Index, TrueLevel[CPColumn[CP].Data[Row].Level], CPColumn[CP].Data[Row].AttackIV, CPColumn[CP].Data[Row].DefenseIV, CPColumn[CP].Data[Row].HPIV);
			}
			++CP;
		}
		++Row;
		(void)fprintf(PokemonOutputFile, "</row>");
	}
	free(EndMySuffering);
	free(CPRowMax);
	//Print the XML footer
	(void)fprintf(PokemonOutputFile, "</sheetData><pageMargins left=\"0.7\" right=\"0.7\" top=\"0.75\" bottom=\"0.75\" header=\"0.3\" footer=\"0.3\"/></worksheet>");
	(void)fclose(PokemonOutputFile);
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		free(CPColumn[CP].Data);
	}
	free(CPColumn);
	CrappyPrintf("\nDone");
}
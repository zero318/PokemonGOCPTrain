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
#define BinaryMode 0
#define CSVMode 1
#define ODSMode 2
#define CalcMode CSVMode

//#define PrintCrap

#ifdef PrintCrap
#define CrappyPrintf(StringyThingy, ...) (void)printf(StringyThingy, __VA_ARGS__);
#else
#define CrappyPrintf(StringyThingy, ...) /*yeet*/
#endif

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
typedef struct PokemonInputStruct {
	uint32_t BaseHP : 10, BaseDefense : 10, BaseAttack : 10, : 0;
	uint16_t Form : 3, Dex : 10, : 3;
} PokemonInputStruct;

typedef union {
	struct PokemonInputStruct;
	uint8_t InChars[6];
} PokemonInputData;

typedef union {
	float single;
	uint8_t InChars[4];
} CPMFloat;

typedef struct PokemonOutputStruct {
	uint32_t HPIV : 4, DefenseIV : 4, AttackIV : 4, Level : 7, Form : 3, Dex : 10;
} PokemonOutputStruct;

typedef union PokemonOutputUnion {
	struct PokemonOutputStruct Struct;
	uint8_t OutChars[4];
} PokemonOutputUnion;

typedef struct {
	uint_fast32_t Count;
	union PokemonOutputUnion* ColumnData;
} CPMatchesData;
#pragma pack(pop)

void main() {
	HANDLE CalculatorProcess = GetCurrentProcess();
	SetPriorityClass(CalculatorProcess, REALTIME_PRIORITY_CLASS);
	CrappyPrintf("Pokemon GO CP Combination Calculator\nPokemon Count Pass 1:\n")
	FILE *PokemonStatsFile = fopen(".\\PokemonStats.bin", "rb");
	double CPMDoublesSquared[MaxLevel];
	{//Lol, I don't need CPMFloats after this
		uint_fast8_t CPM = 0;
		CPMFloat CPMFloats[MaxCPM];
		(void)fread(&CPMFloats->InChars, MaxCPM, sizeof(float), PokemonStatsFile);
		for (uint_fast8_t Level = 0; Level < MaxLevel; ++Level) {
			if (Level & 1) {
#pragma warning(suppress:26451)	//Can VS just shut up about my cast operation? It's supposed to be like that.
				CPMDoublesSquared[Level] = sqrt(CPMFloats[CPM].single * CPMFloats[CPM].single - CPMFloats[CPM].single * CPMFloats[CPM].single / 2 + CPMFloats[CPM + 1].single * CPMFloats[CPM + 1].single / 2);
				++CPM;
			}
			else {
				CPMDoublesSquared[Level] = (double)CPMFloats[CPM].single;
			}
			CPMDoublesSquared[Level] *= CPMDoublesSquared[Level];
		}
	}
	PokemonInputData* PokemonData = SaferMalloc(PokemonInputData, PokemonData, MaxPokemon * sizeof(PokemonInputData))
	//PokemonInputData* PokemonData = SaferCalloc(PokemonInputData, PokemonData, MaxPokemon, sizeof(PokemonInputData))
	(void)fread(PokemonData, MaxPokemon, 6, PokemonStatsFile);
	(void)fclose(PokemonStatsFile);
	//CPMatchesData* CPMatches = SaferMalloc(CPMatchesData, CPMatches, MaxCP * sizeof(CPMatchesData))
	CPMatchesData* CPMatches = SaferCalloc(CPMatchesData, CPMatches, MaxCP, sizeof(CPMatchesData))
	PokemonOutputStruct TempOutputStruct;
	//uint_fast16_t* CPList = (uint_fast16_t*)calloc(MaxPokemon * MaxLevel * IVCount * IVCount * IVCount, sizeof(uint_fast16_t));
	uint16_t* CPList = SaferMalloc(uint16_t, CPList, MaxPokemon * MaxLevel * IVCount * IVCount * IVCount * sizeof(uint16_t))
	//uint16_t* CPList = SaferCalloc(uint16_t, CPList, MaxPokemon * MaxLevel * IVCount * IVCount * IVCount, sizeof(uint16_t))
	uint_fast32_t CPCount = 0;
	for (uint_fast16_t Pokemon = 0; Pokemon < MaxPokemon; ++Pokemon) {
		CrappyPrintf("%u\r", Pokemon);
		for (TempOutputStruct.Level = MinLevel; TempOutputStruct.Level <= MaxLevel; ++TempOutputStruct.Level) {
			for (TempOutputStruct.AttackIV = MinIV; TempOutputStruct.AttackIV < MaxIV; ++TempOutputStruct.AttackIV) {
				for (TempOutputStruct.DefenseIV = MinIV; TempOutputStruct.DefenseIV < MaxIV; ++TempOutputStruct.DefenseIV) {
					for (TempOutputStruct.HPIV = MinIV; TempOutputStruct.HPIV < MaxIV; ++TempOutputStruct.HPIV) {
#pragma warning(suppress:6011)
#pragma warning(suppress:6385)
#pragma warning(suppress:26451)
						CPList[CPCount] = (uint16_t)(((PokemonData[Pokemon].BaseAttack + TempOutputStruct.AttackIV) * sqrt(PokemonData[Pokemon].BaseDefense + TempOutputStruct.DefenseIV) * sqrt(PokemonData[Pokemon].BaseHP + TempOutputStruct.HPIV) * CPMDoublesSquared[TempOutputStruct.Level - 1]) / 10) - 1;
						++CPMatches[CPList[CPCount]].Count;
						++CPCount;
					}
				}
			}
		}
	}
	uint_fast16_t CP;
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		if (CPMatches[CP].Count) {
			CPMatches[CP].ColumnData = SaferMalloc(PokemonOutputUnion, CPMatches[CP].ColumnData, CPMatches[CP].Count * sizeof(PokemonOutputUnion))
			//CPMatches[CP].ColumnData = SaferCalloc(PokemonOutputUnion, CPMatches[CP].ColumnData, CPMatches[CP].Count, sizeof(PokemonOutputUnion))
			CPMatches[CP].Count = 0;
		}
	}
	CrappyPrintf("\nPokemon Count Pass 2:\n")
	CPCount = 0;
	for (uint_fast16_t Pokemon = 0; Pokemon < MaxPokemon; ++Pokemon) {
		CrappyPrintf("%u\r", Pokemon)
		TempOutputStruct.Dex = PokemonData[Pokemon].Dex;
		TempOutputStruct.Form = PokemonData[Pokemon].Form;
		for (TempOutputStruct.Level = MinLevel; TempOutputStruct.Level <= MaxLevel; ++TempOutputStruct.Level) {
			for (TempOutputStruct.AttackIV = MinIV; TempOutputStruct.AttackIV < MaxIV; ++TempOutputStruct.AttackIV) {
				for (TempOutputStruct.DefenseIV = MinIV; TempOutputStruct.DefenseIV < MaxIV; ++TempOutputStruct.DefenseIV) {
					for (TempOutputStruct.HPIV = MinIV; TempOutputStruct.HPIV < MaxIV; ++TempOutputStruct.HPIV) {
						CPMatches[CPList[CPCount]].ColumnData[CPMatches[CPList[CPCount]].Count].Struct = TempOutputStruct;
						++CPMatches[CPList[CPCount]].Count;
						++CPCount;
					}
				}
			}
		}
	}
	free(PokemonData);
	free(CPList);
	CrappyPrintf("\nCPCount:\n")
#if CalcMode == BinaryMode
	FILE* PokemonOutputFile = fopen(".\\PokemonCPOutput.bin", "w+b");
	PokemonOutputUnion ColumnSpacer = { .Struct.HPIV = 0, .Struct.DefenseIV = 0, .Struct.AttackIV = 0, .Struct.Level = 0, .Struct.Form = 0, .Struct.Dex = 0 };
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		CrappyPrintf("%u\r", CP)
		if (CPMatches[CP].Count) {
			for (uint_fast32_t PokemonOutput = 0; PokemonOutput < CPMatches[CP].Count; ++PokemonOutput) {
#pragma warning(suppress:6001)
				(void)fwrite(CPMatches[CP].ColumnData[PokemonOutput].OutChars, 1, 4, PokemonOutputFile);
			}
			free(CPMatches[CP].ColumnData);
			(void)fwrite(ColumnSpacer.OutChars, 1, 4, PokemonOutputFile);
		}
	}
	free(CPMatches);
	(void)fclose(PokemonOutputFile);
#elif CalcMode == CSVMode
	FILE* PokemonOutputFile = fopen(".\\PokemonCPOutput.csv", "w+");
	uint_fast32_t MaxCount = 0;
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		CrappyPrintf("%u\r", CP)
		if (CPMatches[CP].Count) {
			if (CPMatches[CP].Count > MaxCount) {
				MaxCount = CPMatches[CP].Count;
			}
		}
	}
	CrappyPrintf("\nMaxCount: %u\nCount:\n", MaxCount)
	for (uint_fast32_t CountIndex = 0; CountIndex < MaxCount; ++CountIndex) {
		CrappyPrintf("%u\r", CountIndex)
		if (CountIndex < CPMatches[0].Count) {
			(void)fprintf(PokemonOutputFile, "%3u%o%2u%X%X%X", CPMatches[0].ColumnData[CountIndex].Struct.Dex, CPMatches[0].ColumnData[CountIndex].Struct.Form, CPMatches[0].ColumnData[CountIndex].Struct.Level, CPMatches[0].ColumnData[CountIndex].Struct.AttackIV, CPMatches[0].ColumnData[CountIndex].Struct.DefenseIV, CPMatches[0].ColumnData[CountIndex].Struct.HPIV);
		}
		for (CP = MinCP; CP < MaxCP; ++CP) {
			if (CountIndex < CPMatches[CP].Count) {
				(void)fprintf(PokemonOutputFile, ",%3u%o%2u%X%X%X", CPMatches[CP].ColumnData[CountIndex].Struct.Dex, CPMatches[CP].ColumnData[CountIndex].Struct.Form, CPMatches[CP].ColumnData[CountIndex].Struct.Level, CPMatches[CP].ColumnData[CountIndex].Struct.AttackIV, CPMatches[CP].ColumnData[CountIndex].Struct.DefenseIV, CPMatches[CP].ColumnData[CountIndex].Struct.HPIV);
			}
			else {
				(void)fprintf(PokemonOutputFile, ",");
			}
		}
		(void)fprintf(PokemonOutputFile, "\n");
	}
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		free(CPMatches[CP].ColumnData);
	}
	free(CPMatches);
	(void)fclose(PokemonOutputFile);
#elif CalcMode == ODSMode
	FILE* PokemonOutputFile = fopen(".\\PokemonCPOutput.xml", "w+");
	(void)fprintf(PokemonOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n<office:document-content xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\" xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\" xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\" xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\" xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\" xmlns:of=\"urn:oasis:names:tc:opendocument:xmlns:of:1.2\" office:version=\"1.2\"><office:font-face-decls><style:font-face style:name=\"Calibri\" svg:font-family=\"Calibri\"/></office:font-face-decls><office:automatic-styles><style:style style:name=\"ce1\" style:family=\"table-cell\" style:parent-style-name=\"Default\" style:data-style-name=\"N0\"/><style:style style:name=\"co1\" style:family=\"table-column\"><style:table-column-properties fo:break-before=\"auto\" style:column-width=\"1.69333333333333cm\"/></style:style><style:style style:name=\"ro1\" style:family=\"table-row\"><style:table-row-properties style:row-height=\"15pt\" style:use-optimal-row-height=\"true\" fo:break-before=\"auto\"/></style:style><style:style style:name=\"ta1\" style:family=\"table\" style:master-page-name=\"mp1\"><style:table-properties table:display=\"true\" style:writing-mode=\"lr-tb\"/></style:style></office:automatic-styles><office:body><office:spreadsheet><table:calculation-settings table:case-sensitive=\"false\" table:search-criteria-must-apply-to-whole-cell=\"true\" table:use-wildcards=\"true\" table:use-regular-expressions=\"false\" table:automatic-find-labels=\"false\"/><table:table table:name=\"OutputSheet\" table:style-name=\"ta1\">");
	PokemonOutputStruct** PokemonOutput = SaferCalloc(PokemonOutputStruct*, PokemonOutput, MaxCP, sizeof(PokemonOutputStruct*))
	PokemonOutputData* OutputRef = NULL, * OutputRef2;
	uint_fast32_t MaxCount = 0;
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		CrappyPrintf("%u\r", CP)
		if (CPMatches[CP].Count) {
			if (CPMatches[CP].Count > MaxCount) {
				MaxCount = CPMatches[CP].Count;
			}
#pragma warning(suppress:6011)
			PokemonOutput[CP] = SaferCalloc(PokemonOutputStruct, PokemonOutput[CP], CPMatches[CP].Count, sizeof(PokemonOutputStruct))
			OutputRef = CPMatches[CP].StartOfColumn;
			for (uint_fast32_t CountIndex = 0; CountIndex < CPMatches[CP].Count; ++CountIndex) {
#pragma warning(suppress:6001)
				PokemonOutput[CP][CountIndex] = OutputRef->Data.Struct;
				OutputRef2 = OutputRef;
				OutputRef = OutputRef->NextData;
				free(OutputRef2);
			}
		}
	}
	CrappyPrintf("\nMaxCount: %u\nCount:\n", MaxCount)
	for (CP = MinCP; CP <= MaxCP; ++CP) {
		(void)fprintf(PokemonOutputFile, "");
	}
	for (uint_fast32_t CountIndex = 0; CountIndex < MaxCount; ++CountIndex) {
		CrappyPrintf("%u\r", CountIndex)
		if (CountIndex < CPMatches[0].Count) {
			(void)fprintf(PokemonOutputFile, "%3u%o%2u%X%X%X", PokemonOutput[0][CountIndex].Dex, PokemonOutput[0][CountIndex].Form, PokemonOutput[0][CountIndex].Level, PokemonOutput[0][CountIndex].AttackIV, PokemonOutput[0][CountIndex].DefenseIV, PokemonOutput[0][CountIndex].HPIV);
		}
		for (CP = MinCP; CP < MaxCP; ++CP) {
			if (CountIndex < CPMatches[CP].Count) {
				(void)fprintf(PokemonOutputFile, ",%3u%o%2u%X%X%X", PokemonOutput[CP][CountIndex].Dex, PokemonOutput[CP][CountIndex].Form, PokemonOutput[CP][CountIndex].Level, PokemonOutput[CP][CountIndex].AttackIV, PokemonOutput[CP][CountIndex].DefenseIV, PokemonOutput[CP][CountIndex].HPIV);
			}
			else {
				(void)fprintf(PokemonOutputFile, ",");
			}
		}
		(void)fprintf(PokemonOutputFile, "\n");
	}
	free(CPMatches);
	for (CP = MinCP - 1; CP < MaxCP; ++CP) {
		free(PokemonOutput[CP]);
	}
	free(PokemonOutput);
	(void)fclose(PokemonOutputFile);
#endif
	(void)printf("\nDone");
}
//Screw you and your security errors
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

//Fun fact: Comments directly above a macro
//definitions are visible when hovering
//over the macro in the middle of code.
//That's why all macros have some sort of comment

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
#define AbsMinPercentIV 0
#define AbsMaxPercentIV 45

/*Wrapping other macro definitions in
an empty do loop *forces* the macros to
be invoked with a semicolon on the end
without incurring any speed penalties*/
#define DoSomething(OtherMacro) do {OtherMacro} while (0)
/*This literally does nothing*/
#define DoNothing do {} while (0)

/*This prevents excess user input from
"spilling over" into the next input check*/
#define FlushStdIn() while ((getchar()) != '\n')

/*These variable names are defined as macros
to make it slightly more obvious that there's
something abnormal about them, thus
making sure the verbose macros stay intact*/

#define VerboseMode Verbose
#define VerboseDigits VerboseLength
#define DenominatorName VerboseDenominator
#define NumeratorName VerboseNumerator

#define PrintCrap
//Messing with the macros/comments in the header area
//tends to make this section flood with random errors.
//Just Cut/Paste the whole block and they'll go away.
#ifdef PrintCrap
/*This macro is literally just a toggleable printf.
The syntax is the same as the regular printf, but
it can be toggled on/off by defining PrintCrap*/
#define CrappyPrintf(...) DoSomething(printf(__VA_ARGS__);)
/*Like CrappyPrintf, just specifically for verbosity. :P*/
#define VerboseMessage(Label) DoSomething(\
if (VerboseMode) {\
	printf(Label);\
})
/*Prints the " /##" style progress bar
*and* adjusts the spacing of the
VerboseProgress macro to match*/
#define VerboseProgressHeader(Label, Length) DoSomething(\
if (VerboseMode) {\
	printf(Label);\
	VerboseDigits = (char)(floor(log10(Length) + 1) + 48); \
	DenominatorName[1] = VerboseDigits + 1; \
	NumeratorName[2] = VerboseDigits; \
	printf(DenominatorName, '/', Length);\
})
/*Prints progress according to the string
previously set by VerboseProgressHeader*/
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

/*Just malloc with inlined size
calculation and error checking*/
#define SaferMalloc(Type, Pointer, Count) \
(Type*)malloc((size_t)Count * sizeof(Type));\
DoSomething(if (!Pointer) {\
	printf("malloc of size %zu failed!\n", (size_t)Count * sizeof(Type));\
	exit(EXIT_FAILURE);\
})
/*Just calloc with inlined error checking*/
#define SaferCalloc(Type, Pointer, Count) \
(Type*)calloc(Count, sizeof(Type));\
DoSomething(if (!Pointer) {\
	printf("calloc of count %llu and size %zu failed!\n", (size_t)Count, (size_t)sizeof(Type));\
	exit(EXIT_FAILURE);\
})

/*Checks to make sure the file didn't contain illegal values*/
#define MinRangeCheck(Value, AbsValue) DoSomething(\
if (Value < AbsValue) {\
	printf("Error!\n%s(%u) cannot be less than %s(%u)!", #Value, Value, #AbsValue, AbsValue);\
	exit(EXIT_FAILURE);\
})
/*Checks to make sure the file didn't contain illegal values*/
#define MaxRangeCheck(Value, AbsValue) DoSomething(\
if (Value > AbsValue) {\
	printf("Error!\n%s(%u) cannot be greater than %s(%u)!", #Value, Value, #AbsValue, AbsValue);\
	exit(EXIT_FAILURE);\
})
/*Checks to make sure the file didn't contain illegal values*/
#define MinOverMaxCheck(MinValue, MaxValue) DoSomething(\
if (MinValue > MaxValue) {\
	printf("Error!\n%s(%u) cannot be greater than %s(%u)!", #MinValue, MinValue, #MaxValue, MaxValue);\
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
typedef uint_fast8_t CPMIndexer;
typedef uint_fast8_t LevelIndexer;
typedef uint_fast16_t PokemonIndexer;
typedef uint_fast8_t IVIndexer;
typedef uint_fast8_t IVTotal;

#pragma pack(push, 1)
typedef struct PokemonStatsStruct {
	uint32_t BaseHP : 10, BaseDefense : 10, BaseAttack : 10, : 0;
} PokemonStatsStruct;

typedef struct CPComboStruct {
	uint_least32_t HPIV : 5, DefenseIV : 5, AttackIV : 5, Level : 7, Index : 10;
} CPComboStruct;
#pragma pack(pop)

/*More things with inlined error checking.
You may have noticed a trend by now.*/
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

int main() {
	//Set the process priority as high as possible since maybe it'll run faster
	HANDLE CalculatorProcess = GetCurrentProcess();
	SetPriorityClass(CalculatorProcess, REALTIME_PRIORITY_CLASS);
	Flag RunLoop = 1;
	do {//while (RunLoop)
	PokemonIndexer MinPokemonIndex = AbsMinPokemon, MaxPokemonIndex = AbsMaxPokemon, PokemonCount = 0;
	LevelIndexer MinLevel = AbsMinLevel, MaxLevel = AbsMaxLevel, LevelCount;
	IVIndexer MinAttack = MinIV, MaxAttack = MaxIV;
	IVIndexer MinDefense = MinIV, MaxDefense = MaxIV;
	IVIndexer MinHP = MinIV, MaxHP = MaxIV;
	IVTotal MinTotalIV = 0, MaxTotalIV = 45;
	PokemonIndexer PokemonList[AbsPokemonCount];
	{
		FILE* SettingsFile = fopen(".\\DefaultSettings.ini", "rb");
		if (!SettingsFile) {
			SettingsFile = fopen(".\\DefaultSettings.ini", "wb");
			(void)fprintf(SettingsFile, "FlagFPLevel=1\r\nMinPokemon=1\r\nMaxPokemon=719\r\nMinLevel=1\r\nMaxLevel=40\r\nMinAttack=0\r\nMaxAttack=15\r\nMinDefense=0\r\nMaxDefense=15\r\nMinHP=0\r\nMaxHP=15\r\nMin%%IV=41/45\r\nMax%%IV=45/45\r\n");
			(void)freopen(".\\DefaultSettings.ini", "rb", SettingsFile);
		}
		char SettingsBuffer[BUFSIZ];
		PokemonIndexer MinPokemon = 0, MaxPokemon = 0, Exclude = UINT_FAST16_MAX;
		double FPLevel;
		//REEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
		//This is valid code, shut up Visual Studio!
		//https://developercommunityapi.westus.cloudapp.azure.com/content/problem/523033/static-analysis-doesnt-recognize-hh-length-modifie.html
#pragma warning(disable:6328)	
		do {
			fgets(SettingsBuffer, 2, SettingsFile);
			if (SettingsBuffer[0] == 'M') {
				fgets(SettingsBuffer, 3, SettingsFile);
				if (!strcmp(SettingsBuffer, "in")) {
					fgets(SettingsBuffer, BUFSIZ, SettingsFile);
					if (sscanf(SettingsBuffer, "Pokemon=%u", &MinPokemon)) {
						MinRangeCheck(MinPokemon, AbsMinPokemon);
					}
					else if (sscanf(SettingsBuffer, "Attack=%hhu", &MinAttack)) {
						MinRangeCheck(MinAttack, MinIV);
					}
					else if (sscanf(SettingsBuffer, "Defense=%hhu", &MinDefense)) {
						MinRangeCheck(MinDefense, MinIV);
					}
					else if (sscanf(SettingsBuffer, "HP=%hhu", &MinHP)) {
						MinRangeCheck(MinHP, MinIV);
					}
					else if (sscanf(SettingsBuffer, "Level=%lf", &FPLevel)) {
						MinLevel = (LevelIndexer)((FPLevel * 2) - 2);
						MinRangeCheck(MinLevel, AbsMinLevel);
					}
					else if (sscanf(SettingsBuffer, "%%IV=%hhu/45", &MinTotalIV)) {
						MinRangeCheck(MinTotalIV, AbsMinPercentIV);
					}
				}
				else if (!strcmp(SettingsBuffer, "ax")) {
					fgets(SettingsBuffer, BUFSIZ, SettingsFile);
					if (sscanf(SettingsBuffer, "Pokemon=%u", &MaxPokemon)) {
						MaxRangeCheck(MaxPokemon, AbsMaxPokemon);
					}
					else if (sscanf(SettingsBuffer, "Attack=%hhu", &MaxAttack)) {
						MaxRangeCheck(MaxAttack, MaxIV);
					}
					else if (sscanf(SettingsBuffer, "Defense=%hhu", &MaxDefense)) {
						MaxRangeCheck(MaxDefense, MaxIV);
					}
					else if (sscanf(SettingsBuffer, "HP=%hhu", &MaxHP)) {
						MaxRangeCheck(MaxHP, MaxIV);
					}
					else if (sscanf(SettingsBuffer, "Level=%lf", &FPLevel)) {
						MaxLevel = (LevelIndexer)((FPLevel * 2) - 2);
						MaxRangeCheck(MaxLevel, AbsMaxLevel);
					}
					else if (sscanf(SettingsBuffer, "%%IV=%hhu/45", &MaxTotalIV)) {
						MaxRangeCheck(MaxTotalIV, AbsMaxPercentIV);
					}
				}
			}
			else if (SettingsBuffer[0] == 'I') {
				fgets(SettingsBuffer, 8, SettingsFile);
				if (!strcmp(SettingsBuffer, "nclude=")) {
					fgets(SettingsBuffer, BUFSIZ, SettingsFile);
					char* Delim = ",";
					char* PokemonToken = strtok(SettingsBuffer, Delim);
					while (PokemonToken) {
						PokemonIndexer MinPokeBound, MaxPokeBound;
						switch (sscanf(PokemonToken, "%u-%u", &MinPokeBound, &MaxPokeBound)) {
							case 1:
								MaxPokeBound = MinPokeBound;
							case 2:
								--MinPokeBound;
								--MaxPokeBound;
								MinOverMaxCheck(MinPokeBound, MaxPokeBound);
								for (PokemonIndexer Pokemon = MinPokeBound; Pokemon <= MaxPokeBound; ++Pokemon) {
									PokemonList[PokemonCount] = Pokemon;
									++PokemonCount;
								}
						}
						PokemonToken = strtok(NULL, Delim);
					}
				}
			}
			else if (SettingsBuffer[0] == 'E') {
				fgets(SettingsBuffer, 8, SettingsFile);
				if (!strcmp(SettingsBuffer, "xclude=")) {
					fgets(SettingsBuffer, 4, SettingsFile);
					if (sscanf(SettingsBuffer, "%u", &Exclude)) {
						--Exclude;
					}
				}
			}
			else {
				fgets(SettingsBuffer, BUFSIZ, SettingsFile);
			}
		} while (!feof(SettingsFile));
#pragma warning(default:6328)
		(void)fclose(SettingsFile);
		MinOverMaxCheck(MinPokemon, MaxPokemon);
		MinOverMaxCheck(MinLevel, MaxLevel);
		MinOverMaxCheck(MinAttack, MaxAttack);
		MinOverMaxCheck(MinDefense, MaxDefense);
		MinOverMaxCheck(MinHP, MaxHP);
		MinOverMaxCheck(MinTotalIV, MaxTotalIV);
		LevelCount = (MaxLevel - MinLevel + 1);
		if (!PokemonCount) {
			printf("No pokemon selected!");
			exit(EXIT_FAILURE);
		}
		if (Exclude != UINT_FAST16_MAX) {
			for (PokemonIndexer PokemonListIndex = 0; PokemonListIndex < PokemonCount; ++PokemonListIndex) {
				if (PokemonList[PokemonListIndex] == Exclude) {
					memmove(&PokemonList[PokemonListIndex], &PokemonList[PokemonListIndex + 1], ((PokemonCount - PokemonListIndex) - 1) * sizeof(PokemonIndexer));
					--PokemonCount;
					break;
				}
			}
		}
		for (PokemonIndexer PokemonListIndex = 0; PokemonListIndex < PokemonCount; ++PokemonListIndex) {
			if (PokemonList[PokemonListIndex] >= MinPokemon) {
				MinPokemonIndex = PokemonListIndex;
				break;
			}
		}
#pragma warning(suppress:6295)
		for (PokemonIndexer PokemonListIndex = PokemonCount - 1; PokemonListIndex >= 0; --PokemonListIndex) {
#pragma warning(suppress:6001)
#pragma warning(suppress:6385)
			if (PokemonList[PokemonListIndex] <= MaxPokemon) {
				MaxPokemonIndex = PokemonListIndex;
				break;
			}
		}
		PokemonCount = (MaxPokemonIndex - MinPokemonIndex) + 1;
		if (!PokemonCount) {
			printf("No pokemon selected!");
			exit(EXIT_FAILURE);
		}
	}
	do {//while (RunLoop == 2)
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
	{
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
	do {//while (RunLoop == 3)
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
	PokemonStatsStruct* PokemonStats = (PokemonStatsStruct*)SaferResourceLoad(PokemonStats2File);
	FastCPValue CP;
	uint_fast16_t AttackStats[IVCount];
	double DefenseStats[IVCount], HPStats[IVCount], AttackDefense;
	VerboseProgressHeader("\nPokemon Count Pass 1:\n", PokemonCount);
	for (PokemonIndexer PokemonListIndex = MinPokemonIndex; PokemonListIndex <= MaxPokemonIndex; ++PokemonListIndex) {
		VerboseProgress(PokemonListIndex);
		PokemonIndexer Pokemon = PokemonList[PokemonListIndex];
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
#pragma warning(suppress:6386)
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
	VerboseProgressHeader("\nPokemon Count Pass 2:\n", PokemonCount);
	CPIndex = 0;
	//This struct is used for holding many temporary values
	CPComboStruct Combo;
	for (PokemonIndexer PokemonListIndex = MinPokemonIndex; PokemonListIndex <= MaxPokemonIndex; ++PokemonListIndex) {
		VerboseProgress(PokemonListIndex);
		Combo.Index = PokemonList[PokemonListIndex];
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
	//XLSX mode
	if (OutputMode == 0) {
		//Load a file with a bunch of strings representing Excel's column headers
		//Because screw trying to actually calcuate those dang letters on the fly
		//All indices are multiplied by 4 to account for max string length
		char* EndMySuffering2 = (char*)SaferResourceLoad(FMLFile);
		//This is probably just me being superstitious, but I feel like it's faster to read from an a malloc array than an
		//embedded file, so copy all the data to an array and then just read from that instead
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
		free(EndMySuffering);
		//Print the XML footer
		(void)fputs("</sheetData><pageMargins left=\"0.7\" right=\"0.7\" top=\"0.75\" bottom=\"0.75\" header=\"0.3\" footer=\"0.3\"/></worksheet>", GenericFile);
		(void)fclose(GenericFile);
	}
	//Text mode
	else {
		--SpecificCP;
		if (SpecificCP < MinCP || SpecificCP > MaxCP) {
			printf("\nSpecified CP is outside the calcualted range!");
			exit(EXIT_FAILURE);
		}
		if (!CPColumnHeight[SpecificCP]) {
			printf("\nNo results for specified CP!");
			exit(EXIT_FAILURE);
		}
		char* PokemonNames = (char*)SaferResourceLoad(PokemonNamesFile);
		double TrueLevel[89] = { 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0, 12.5, 13.0, 13.5, 14.0, 14.5, 15.0, 15.5, 16.0, 16.5, 17.0, 17.5, 18.0, 18.5, 19.0, 19.5, 20.0, 20.5, 21.0, 21.5, 22.0, 22.5, 23.0, 23.5, 24.0, 24.5, 25.0, 25.5, 26.0, 26.5, 27.0, 27.5, 28.0, 28.5, 29.0, 29.5, 30.0, 30.5, 31.0, 31.5, 32.0, 32.5, 33.0, 33.5, 34.0, 34.5, 35.0, 35.5, 36.0, 36.5, 37.0, 37.5, 38.0, 38.5, 39.0, 39.5, 40.0, 40.5, 41.0, 41.5, 42.0, 42.5, 43.0, 43.5, 44.0, 44.5, 45.0 };
		IVTotal DisplayPercents[46] = { 0, 2, 4, 6, 8, 11, 13, 15, 17, 20, 22, 24, 26, 28, 31, 33, 35, 37, 40, 42, 44, 46, 48, 51, 53, 55, 57, 60, 62, 64, 66, 68, 71, 73, 75, 77, 80, 82, 84, 86, 88, 91, 93, 95, 97, 100 };
		FILE* CPOutputFile = fopen(".\\CPOutput.txt", "w+b");
		VerboseProgressHeader("\nRow Parsing 1:\n", CPColumnHeight[SpecificCP] - 1);
		CPComboStruct TempStruct;
		IVTotal TotalIV;
		(void)fprintf(CPOutputFile, "Pokemon Name            Lvl   IV%% At/Df/HP\r\n覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧");
		for (CPComboCount Row = 0; Row < CPColumnHeight[SpecificCP]; ++Row) {
			VerboseProgress(Row);
			TempStruct = CPColumn[SpecificCP][Row];
			TotalIV = TempStruct.AttackIV + TempStruct.DefenseIV + TempStruct.HPIV;
			if (TotalIV >= MinTotalIV && TotalIV <= MaxTotalIV) {
				(void)fprintf(CPOutputFile, "\r\n%-23s %4.1f %3hhu%% %02u/%02u/%02u", &PokemonNames[TempStruct.Index * 24], TrueLevel[TempStruct.Level], DisplayPercents[TotalIV], TempStruct.AttackIV, TempStruct.DefenseIV, TempStruct.HPIV);
			}
		}
		(void)fclose(CPOutputFile);
	}
	for (CP = 0; CP <= MaxCP; ++CP) {
		if (CPColumnHeight[CP]) {
			free(CPColumn[CP]);
		}
	}
	free(CPColumnHeight);
	free(CPColumn);
	VerboseMessage("\n");
	{
		printf("Done.\n"
			   "1. Return to main menu\n"
			   "2. Return to main menu and reload settings\n"
			   "3. Rerun the last operation\n"
			   "4. Exit\n"
			   "C");
		Flag UserIsDumb = 0;
		int ModeSelect = 0;
		char Nope;
		do {
			printf("hoose an option: ");
			UserIsDumb = scanf("%1u%1[^\n]", &ModeSelect, &Nope);
			FlushStdIn();
			if (UserIsDumb != 1) {
				printf("*Properly* c");
			}
		} while (UserIsDumb != 1);
		switch (ModeSelect) {
		case 1:
			RunLoop = 2;
			break;
		case 2:
			RunLoop = 1;
			break;
		case 3:
			RunLoop = 3;
			break;
		case 4:
			RunLoop = 0;
			break;
		default:
			printf("That wasn't an option. :P");
			exit(EXIT_SUCCESS);
		}
	}
	} while (RunLoop == 3);
	} while (RunLoop == 2);
	} while (RunLoop);
	exit(EXIT_SUCCESS);
}
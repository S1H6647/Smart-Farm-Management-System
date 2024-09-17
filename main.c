#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_RED      "\x1b[31m"
#define COLOR_RESET    "\x1b[0m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_CYAN    "\x1b[36m"

#define MAX_CROPS 100
#define MAX_EXPENSES 100
#define MOISTURE_THRESHOLD 40
#define MAX_FIELD 100


/**
 * ! Structure defining:
 */

struct Crops
{
    char cropName[100];
    float area; // Planted in acres
    float expectedYield; // In tons
    char plantingDate[11]; // in format: YYYY-MM-DD
    char harvestingDate[11]; // in format: YYYY-MM-DD
    char status[20]; // Planted, Ready to Harvest, Harvested
} Crops[MAX_CROPS];

// Data for multiple Fields
struct Field
{
    float fieldArea;
    float moistureLevel;
} Field[MAX_FIELD];

// Data for crop expenses only
struct Expenses
{
    char category[100];
    char description[256];
    float amount;
} Expenses[MAX_EXPENSES];

/**
 * ! Necessary Functions:
 */

// The main menu
void menu(); 

//Manage Crops: This section of function allows the farmer to add, view, update and delete crop:
void manageCrops();
void addCrop();
void viewCrop(int index);
void updateCrop(int updateindex);
void deleteCrop(int deleteindex);

//Irrigation Scheduling: This section of function allows to input field data, calculate irrigation, water requirement calculation and generate irrigation schedule.
void irrigationScheduling();
void irrigationNeedCalc(int fieldToCalc);
float waterRequriementCalc(int fieldToCalc);

//Expense Tracking: This sectino of function allows to add expense, view monthly expense, calculate total & average and expense log.
void expenseTracking();
void addExpense();
void viewExpense(); // Monthly
float calcTotal(int expenseCount); // Includes Total and Average
void expenseLog();
void question();

// Fild handling:
void loadFromFile();
void saveToFile();

// Exiting the program
void Exit();

int cropCount = 0, fieldCount = 0,expenseCount = 0;
float total = 0,average = 0;

/**
 * ! Program Starts: 
 */
int main()
{
    // Heading - Introduction about the project:
    printf(COLOR_CYAN "~~~ Smart Farm Management System ~~~\n"COLOR_RESET);
    loadFromFile();
    menu();
    printf(COLOR_RESET"");
    printf("\n");
    system("pause");
    return 0;
}

/**
 * ! File handling functions:
 */
void loadFromFile()
{
    FILE *fp;
    /**
     *  ! Load Crops
     */
    fp = fopen("crops.txt","r");
    if (fp != NULL)
    {
        fscanf(fp, "Number of Crops: %d", &cropCount);
        for(int i = 0; i < cropCount; i++)
        {
            fscanf(fp, "Crop Name: %s\n",Crops[i].cropName); 
            fscanf(fp, "Area (in acres): %.2f\n", Crops[i].area);  
            fscanf(fp, "Expected Yield (in tons): %.2f\n", Crops[i].expectedYield); 
            fscanf(fp, "Planting Date: %s\n", Crops[i].plantingDate); 
            fscanf(fp, "Harvesting Date: %s\n", Crops[i].harvestingDate); 
            fscanf(fp, "Status: %s\n", Crops[i].status);
        }
        fclose(fp);
    }

    /**
     * ! Load Crops
     */
    fp = fopen("field.txt","r");
    if(fp != NULL)
    {
        fscanf(fp,"Number of fields: %d",&fieldCount);
        for (int i = 0; i < fieldCount; i++)
        {
            fscanf(fp, "%.2f", Field[i].fieldArea);
            fscanf(fp, "%.2f", Field[i].moistureLevel);
        }
        fclose(fp);
    }

    /**
     * ! Load Expenses
     */
    fp = fopen("expenses.txt", "r");
    if (fp != NULL) {
        fscanf(fp, "Expense Count: %d", &expenseCount);
        for (int i = 0; i < expenseCount; i++) {
            fscanf(fp, "%s\n", Expenses[i].category);
            fscanf(fp, "%s\n", Expenses[i].description);
            fscanf(fp, ".2%f\n", Expenses[i].amount);
        }
        fclose(fp);
    }
    printf(COLOR_GREEN "\nData Loaded Successfully!\n" COLOR_RESET);
}

void saveToFile() {
    FILE *fp;

    /** 
     * ! Save Crops
     */ 

    fp = fopen("crops.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d\n", cropCount);
        for (int i = 0; i < cropCount; i++) {
            fprintf(fp, "Crop Name: %s\n",Crops[i].cropName); 
            fprintf(fp, "Area (in acres): %.2f\n", Crops[i].area);  
            fprintf(fp, "Expected Yield (in tons): %.2f\n", Crops[i].expectedYield); 
            fprintf(fp, "Planting Date: %s\n", Crops[i].plantingDate); 
            fprintf(fp, "Harvesting Date: %s\n", Crops[i].harvestingDate); 
            fprintf(fp, "Status: %s\n\n", Crops[i].status);
        }
        fclose(fp);
    }

    /** 
     * ! Save Fields
     */

    fp = fopen("fields.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d\n", fieldCount);
        for (int i = 0; i < fieldCount; i++) {
            fprintf(fp, "Field Area: %.2f\n", Field[i].fieldArea);
            fprintf(fp, "Moisture Level: %.2f\n\n", Field[i].moistureLevel);
        }
        fclose(fp);
    }

    /**
     * ! Save Expenses
     */ 
    fp = fopen("expenses.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d\n", expenseCount);
        for (int i = 0; i < expenseCount; i++) 
        {
            fprintf(fp, "Category: %s\n", Expenses[i].category);
            fprintf(fp, "Description: %s\n", Expenses[i].description);
            fprintf(fp, "Amount Spent: %.2f\n\n", Expenses[i].amount);
        }
        fclose(fp);
    }

    printf(COLOR_GREEN "\nData Saved Successfully!\n" COLOR_RESET);
}

/**
 * ! Menu
 */
void menu()
{
    int choice;
    do
    {
        printf("\n");
        printf(COLOR_CYAN"Main Menu: \nEnter the number to choose the option: \n"COLOR_RESET);
        printf("1. Manage Crops\n");
        printf("2. Irrigation Scheduling\n");
        printf("3. Expense Tracking\n");
        printf("4. Save and Exit\n");
        printf("Enter the number to choose the option: ");
        scanf("%d",&choice);
        printf("\n");

        switch (choice)
        {
            case 1:
                manageCrops();
                break;

            case 2:
                printf(COLOR_CYAN"~~~ Irrigation Scheduling Management ~~~\n"COLOR_RESET);
                irrigationScheduling();
                break;

            case 3:
                expenseTracking();
                break;

            case 4: 
                Exit();
                break;

            default:
                printf(COLOR_RED"Wrong Input!\n");
                menu();
                break;
        }
    }while(choice != 4);
}
/**
 * ! Manage Crops:
 */
void manageCrops()
{
    int choice;
    printf(COLOR_CYAN"~~~ Crop Management Section ~~~ \n"COLOR_RESET);
    printf("1. Add Crops Details\n");
    printf("2. View All Crops\n");
    printf("3. Update Crop Status\n");
    printf("4. Delete Crops\n");
    printf("5. Exit to Main Menu\n");
    printf("Enter the number to choose the option: "); 
    printf("\n");
    scanf("%d",&choice);

    switch (choice)
    {
    case 1:
        addCrop();
        break;
    
    case 2:
        if (cropCount == 0)
        {
            printf(COLOR_RED "\nNo crop details found! Please enter crop details before viewing!\n" COLOR_RESET "\n");
            manageCrops();
        }
        else 
        for (int viewindex = 0; viewindex < cropCount; viewindex++)
        {
            viewCrop(viewindex);
        }
        manageCrops();
        break; 

    case 3:
        if (cropCount == 0)
        {
            printf(COLOR_RED "\nNo crop details found! Please enter crop details before updating!\n" COLOR_RESET "\n");
            manageCrops();
        }
        else 
        for (int updateindex = 0; updateindex < cropCount; updateindex++)
        {
            if(updateindex >= 0 && updateindex < cropCount && strcmp(Crops[updateindex].status, "DELETED") != 0) 
            {
                updateCrop(updateindex);
            } else {
                printf(COLOR_RED" The Data has been deleted! \n"COLOR_RESET);
            }
        }
        printf("\n");
        manageCrops();
        break; 

    case 4: 
        if (cropCount == 0)
        {
            printf(COLOR_RED "\nNo crop details found! Please enter crop details before deleting!\n" COLOR_RESET "\n");
            manageCrops();
        }
        else
        for (int deleteindex = 0; deleteindex < cropCount; deleteindex++)
        {
            deleteCrop(deleteindex);
        }
        break; 

    case 5:
        menu();
        break;

    default:
        break;
    }
}

// Add Crops Function: This function adds crop details to the array.
void addCrop()
{
    int input,temp = 0;
    printf(COLOR_GREEN"\nHow many inputs do you want?\n"COLOR_RESET);
    scanf("%d",&input);
    printf("\n");
    while (temp<input)
    {
        printf("Input %d:\n",(cropCount+1));
        printf("Enter Crop Name: \n");
        scanf("%s",Crops[cropCount].cropName);

        printf("Enter Area(in acres): \n");
        scanf("%f",&Crops[cropCount].area);

        printf("Enter your expected yield(in tons): \n");
        scanf("%f",&Crops[cropCount].expectedYield);

        printf("Enter Planting date (YYYY-MM-DD): \n");
        scanf("%s",Crops[cropCount].plantingDate);

        printf("Enter Harvest date (YYYY-MM-DD): \n");
        scanf("%s",Crops[cropCount].harvestingDate);

        printf("Enter the status of Crop: \n");
        scanf("%s",Crops[cropCount].status);
        printf("\n");

    temp++;
    cropCount++;
    }
    manageCrops();
}

// View Crop Information
void viewCrop(int index) // value in i comes and sits here
{
    if (index<cropCount && strcmp(Crops[index].status, "DELETED") != 0)
    {
        printf(COLOR_GREEN"\nViewing crop %d:\n", (index+1));
        printf(COLOR_RESET"");
        printf("Crop Name: %s\n",Crops[index].cropName);
        printf("Area(in acres): %f\n",Crops[index].area);
        printf("Expected Yield(in tons): %f\n",Crops[index].expectedYield);
        printf("Planting Date: %s\n",Crops[index].plantingDate);
        printf("Harvesting Date: %s\n",Crops[index].harvestingDate);
        printf("Crop Status: %s\n",Crops[index].status);
        printf("\n");
    }
    else 
    {
        printf(COLOR_RED "\nNo crop details found! Please enter crop details before viewing!\n" COLOR_RESET "\n");
    }
}

// Update Crop Status
void updateCrop(int updateindex)
{
    int choice;
    if (cropCount == 0)
    {
        printf(COLOR_RED "\nNo crop details found! Please enter crop details before updating!\n" COLOR_RESET "\n");
        manageCrops();
    }
    else 
    {
        printf(COLOR_GREEN"\nEnter the index of crop you want to update (Starting from 0): \n"COLOR_RESET);
        scanf("%d",&updateindex);
        if (updateindex < 0 || updateindex > cropCount)
        {
            printf(COLOR_RED"Invalid index!\n");
            updateCrop(updateindex); // Retrying update
        }
    }
    printf("\n");
    printf(COLOR_GREEN"Which data do you want to update: \n"COLOR_RESET);
    printf("1. Crop Name \n");
    printf("2. Area \n");
    printf("3. Expected Yield \n");
    printf("4. Planting Date \n");
    printf("5. Harvesting Date \n");
    printf("6. Status\n");
    scanf("%d",&choice);

    switch (choice)
    {
    case 1:
        printf("Enter the Updated Crop Name: ");
        scanf("%s",Crops[updateindex].cropName);
        printf(COLOR_GREEN"Your data has been updated!\n"COLOR_RED);
        viewCrop(updateindex);
        break;
    
    case 2:
        printf("Enter the Updated Area: ");
        scanf("%f",&Crops[updateindex].area);
        printf(COLOR_GREEN"Your data has been updated!\n"COLOR_RED);
        viewCrop(updateindex);
        break;

    case 3:
        printf("Enter the Updated Yielding: ");
        scanf("%f",&Crops[updateindex].expectedYield);
        printf(COLOR_GREEN"Your data has been updated!\n"COLOR_RED);
        viewCrop(updateindex);
        break;

    case 4:
        printf("Enter the Updated Planting Date: ");
        scanf("%s",Crops[updateindex].plantingDate);
        printf(COLOR_GREEN"Your data has been updated!\n"COLOR_RED);
        viewCrop(updateindex);
        break;

    case 5:
        printf("Enter the Harvesting Date: ");
        scanf("%s",Crops[updateindex].harvestingDate);
        printf(COLOR_GREEN"Your data has been updated!\n"COLOR_RED);
        viewCrop(updateindex);
        break;

    case 6:
        printf("Enter the Updated Status of Crop: ");
        scanf("%s",Crops[updateindex].status);
        printf(COLOR_GREEN"Your data has been updated!\n"COLOR_RED);
        viewCrop(updateindex);
        break;
    default:
        printf("Wrong Input!\n");
        updateCrop(updateindex);
        break;
    }
    manageCrops();
}

void deleteCrop(int deleteindex)
{
    int choice;
    if (cropCount == 0)
    {
        printf(COLOR_RED "\nNo crop details found! Please enter crop details before Deleting!\n" COLOR_RESET "\n");
        manageCrops();
    }
    else
    {
        if (deleteindex<0 || deleteindex>=cropCount)
        {
            printf(COLOR_RED"Invalid index! \n"COLOR_RESET);
        }
    }

    printf(COLOR_GREEN"Which crop index do you want to delete (Starting from 0): "COLOR_RESET);
    scanf("%d",&deleteindex);
    printf("\n");
    if (deleteindex >= cropCount || deleteindex < 0)
    {
        printf(COLOR_RED"Invalid index!\n"COLOR_RESET);
        printf("\n");
    }
    else
    {
        printf(COLOR_RED"Deleting Crop Date...\n");
        strcpy(Crops[deleteindex].status, "DELETED");
        printf(COLOR_RED"Crop details of %d index has been deleted!\n",deleteindex);
        printf(COLOR_RESET "\n");
    }
    manageCrops();
}

/**
 * ! Irrigation Scheduling
 */
void irrigationScheduling()
{
    /**
     * ! Input Field Data:
     */
    int numFields,i,fieldToCalc,ask;
    float waterRequired;
    char input;

    printf(COLOR_GREEN"Do you want to enter the number of Fields or Exit to Main Menu: \n"COLOR_RESET);
    printf("Enter 1 to Continue and 2 to Exit: ");
    scanf("%d",&ask);
    if (ask == 2)
    {
        menu();
    }

    printf("Enter the number of Fields: \n");
    scanf("%d", &numFields);

    for (i = 0; i < numFields; i++)
    {
        printf(COLOR_GREEN"\nField %d\n",(i+1));
        printf(COLOR_RESET);
        printf("Enter the Area of Field:\n");
        scanf("%f", &Field[fieldCount].fieldArea);
        printf("Enter the Current Soil Moisture of Field:\n");
        scanf("%f", &Field[fieldCount].moistureLevel);
        fieldCount++;
    }
    printf(COLOR_GREEN"\nWhich Field number do you want to check moisture of (Starting from 0): \n"COLOR_RESET);
    scanf("%d",&fieldToCalc);
    irrigationNeedCalc(fieldToCalc);

    printf(COLOR_GREEN"\nWhich Field number do you want water requirement of (Starting from 0): \n"COLOR_RESET);
    scanf("%d",&fieldToCalc);

    waterRequired = waterRequriementCalc(fieldToCalc);
    if (waterRequired>0)
    {
        printf(COLOR_RED"Water Required for Field %d: %.2f litres\n",(i+1), waterRequired);
        printf(COLOR_RESET);
    }
    else
    {
        printf(COLOR_CYAN"Water Not Required for Field %d as Soil Moisture %.2f is greater than Threshold Moisture\n",i,Field[fieldToCalc].moistureLevel);
        printf(COLOR_RESET);
    }
    printf("\n");
    menu();
}

/**
 * ! Calculating whether irrigation is needed or not
 */
void irrigationNeedCalc(int fieldtoCalc)
{
    if(fieldtoCalc<0 || fieldtoCalc>fieldCount)
    {
        printf(COLOR_RED"Invalid Index!\n"COLOR_RESET);
    }
    else
    {
        if (Field[fieldtoCalc].moistureLevel < MOISTURE_THRESHOLD)
        {
            printf(COLOR_RED"\nIrrigation Required! Moisture level is below Moisture Threshold: %.3f\n",Field[fieldtoCalc].moistureLevel);
            printf(COLOR_RESET);
        }
        else
        {
            printf(COLOR_CYAN"\nNo Irrigation Needed! Moisture level is sufficient %.2f\n",Field[fieldtoCalc].moistureLevel);
            printf(COLOR_RESET);
        }
    }
}

/**
 * ! Calculating whether water is required or not
 */
float waterRequriementCalc(int fieldToCalc)
{
    if(fieldToCalc<0 || fieldToCalc >= fieldCount)
    {
        printf(COLOR_RED"Invalid Field Index!\n"COLOR_RESET);
        return 0;
    }

    float moistureDeflict;
    moistureDeflict = MOISTURE_THRESHOLD - Field[fieldToCalc].moistureLevel; // areaIndex == moistureIndex
    if (moistureDeflict > 0)
    {
        float waterRequired =  (Field[fieldToCalc].fieldArea * (100 - Field[fieldToCalc].moistureLevel) * 10);
        return waterRequired;   
    }
    return 0; // If soil moisture is sufficent
}

/**
 * ! Expense tracking: Adding, viewing and calculation
 */
void expenseTracking()
{
    int ask;
    printf(COLOR_CYAN"~~~ Expense Tracking Section ~~~ \n"COLOR_RESET);
    printf(COLOR_GREEN"Do you want to Continue or Exit to Main Menu: \n"COLOR_RESET);
    printf("Enter 1 to Continue and 2 to Exit: ");
    scanf("%d",&ask);
    if (ask == 2)
    {
        menu();
    }
    question();
    printf("\n");
    question();
}

void question()
{
    int choice;
    printf("\n1. Add Expense\n");
    printf("2. View Expense by Category\n");
    printf("3. Calculate Total & Average Expense\n");
    printf("4. Show Expense Log\n");
    printf("5. Exit to Main Menu\n");
    printf("Enter the number to choose the option: ");
    scanf("%d",&choice);
    switch (choice)
    {
    case 1: 
        addExpense();
        break;

    case 2:
        viewExpense();
        question();
        break;
    
    case 3: 
        calcTotal(expenseCount);
        break; 
    
    case 4: 
        expenseLog();
        break;
      
    case 5: 
        menu();
        break;

    default:
        question();
        break;
    }
}
void addExpense()
{
    int input;
    if (expenseCount >= MAX_EXPENSES) 
    {
        printf(COLOR_RED"Cannot add more expenses.\n");
        printf("\n");
        menu();
    }

    printf("\nHow many categories do you want to enter: ");
    scanf("%d",&input);

    for (int i = 0; i< input; i++)
    {
        printf(COLOR_GREEN"\nExpense %d\n",(i+1));
        printf(COLOR_RESET"Enter expense category: ");
        scanf("%s", Expenses[expenseCount].category);
        printf("Enter description: ");
        scanf("%s",Expenses[expenseCount].description);
        printf("Enter amount: ");
        scanf("%f", &Expenses[expenseCount].amount);
        expenseCount++;
        printf(COLOR_GREEN"Expense %d added successfully!\n",(i+1));
        printf(COLOR_RESET);
    }
}

void viewExpense()
{
    if (expenseCount == 0)
    {
        printf(COLOR_RED"\nNo Expenses Recorded!\n"COLOR_RESET);
    }
    else
    {
        for (int i = 0; i < expenseCount; i++)
        {
            printf("\nSummary for Expense %d\n",(i+1));
            printf(COLOR_RESET"Category: %s \nDescription: %s \nAmount: %.2f\n",
            Expenses[i].category,Expenses[i].description,Expenses[i].amount);
            printf("\n");
            question();
        }
    }
}

float calcTotal(int expenseCount)
{
    int choice;
    if (expenseCount == 0)
    {
        printf(COLOR_RED"Please Enter the data before Calculating!"COLOR_RESET);
    }
    printf("\n");
    do
    {
        printf(COLOR_RESET"1. Total Expense\n");
        printf("2. Average Expense\n");
        printf("Enter the number to choose the option: ");
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
            for (int i = 0; i < expenseCount;i++)
            {
                total += Expenses[i].amount; 
            }
            printf(COLOR_GREEN"Total Expense: %.2f\n",total);
            printf(COLOR_RESET);
            break;

        case 2:
            for (int i = 0; i < expenseCount; i++) 
            {
                total += Expenses[i].amount;  
            }
            average = total / expenseCount;
            printf(COLOR_GREEN"Average: %.2f\n\n",average);
            printf(COLOR_RESET);
            break;
        
        default:
            printf(COLOR_RED "Invalid choice!\n" COLOR_RESET);
            break;
        }
        return total;
    }while (choice != 2);
    menu();
}

void expenseLog()
{
    if (expenseCount<0)
    {
        printf(COLOR_RED"Please Enter the Datas First!\n"COLOR_RESET);
    }

    int i;
    for (i = 0; i < expenseCount; i++)
    {
        printf(COLOR_GREEN"\nExpense Log: \n");
        printf(COLOR_RESET"Category: %s\n",Expenses[i].category);
        printf("Description: %s\n",Expenses[i].description);
        printf("Amount Spent: %.2f\n",Expenses[i].amount);
        printf("\n");
    }
    expenseTracking();
}

/**
 * ! Exit
 */
void Exit()
{
    saveToFile();
    printf(COLOR_CYAN"Exiting the program...\n"COLOR_RESET);
    exit(0);
}


#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>
#include <mem.h>

// Project 1        Koval Maksym        xkoval20

//Napsal jsem všechny funkce z úpravy tabulky, povinne zpracování dat, csum,cavg,cmin,cmax a udělal selekce řádků

//Taky jsem udělal error handling

//  Reads string from stdin
bool read_str(char *string, char *delim, int *delim_number);

//  Writes current string in stdout
void write_in_file(char *string);

//  The function checks for the presence of "irow" and returns false if there is no need to read the next line
bool check_arguments_part1(char *string, int argc, char **argv,
                           char *delim, int *line_number,
                           int *delim_number, bool read);

//  The function checks for function calls and returns false if there is no need to read the next line
int check_arguments_part2(char *string, int argc, char **argv, char *buffer, char *delim, int *delim_number, bool run,
                          int *line_number);

// Checks the existence of the "-d" parameter and assigns a divisor
char* check_and_set_delim(char *argv[]);

//  Creates string of delims        Example: "::::::"
void create_str(char *string, char *delim, int *line_number);

//  Counts delimeters in string
bool count_delims(char *string, char *delim, int *delim_number);

//  Adds a new table row to the end of the table
void arow(char *string, char *delim, int *delim_number,
          bool run);

//  Inserts an empty column before the column given by the number C
int icol(char *string, char *buffer, char *delim,
         int *delim_number, char *c);

//  Adds an empty column after the last column
int acol(char *string, char *delim, int *delim_number);

//  Deletes the column given by the number C
void dcol(char *string, char *buffer, char *delim,
          char *c, int *delim_number);

//  The string STR will be set to the cell in column C.
int cset(char *string, char *buffer, char *delim, char *c, char *STR, int *delim_number);

//  The string in column C will be converted to lowercase
void to_lower(char *string, char *delim, char *c);

//  The string in column C will be converted to uppercase
void to_upper(char *string, char *delim, char *c);

//  In column C, rounds the number to an integer
void my_round(char *string, char *buffer, char *delim,
              int *delim_number, char *c);

//  Removes the decimal part of the number in column C
void my_int(char *string, char *buffer, char *delim, int *delim_number, char *c);

//  Copies the values of the cells in columns N and M
int copy(char *string, char *buffer, char *delim, int *delim_number, char *n, char *m);

// Swaps the values of the cells in columns N and M
void swap(char *string, char *buffer, char *delim, int *delim_number, char *n, char *m);

//  Moves column N before column M
void move(char *string, char *buffer, char *delim, int *delim_number, char *n, char *m);

//  The processor will process only those rows whose cell contents in column C begin with a string STR
bool beginwith(char *string, char *buffer, char *delim,
               char *c, char *STR);

//  The processor will process only those rows whose cells in column C contain the string STR
bool contains(char *string, char *buffer, char *delim,
              char *c, char *STR);

//  Sums all cells between N and M and assigns it to C
int csum(char *string, char *buffer, char *delim, int *delim_number, char *c, char *n, char *m);

//  Finds the average value between N and M and assigns it to C
void cavg(char *string, char *buffer, char *delim, int *delim_number, char *c, char *n, char *m);

//  Finds the minimal value between N and M and assigns it to C
void cmin(char *string, char *buffer, char *delim, int *delim_number, char *c, char *n, char *m);

//  Finds the maximal value between N and M and assigns it to C
void cmax(char *string, char *buffer, char *delim, int *delim_number, char *c, char *n, char *m);

#define MAX_ARRAY_SIZE 10240        //      10KiB + '\0'

#define MAX_IN_BOX 100

#define MIN_ARGUMENTS 1

int main(int argc, char *argv[])
{

    char string[MAX_ARRAY_SIZE];            //      Main array with current string
    char buffer[MAX_ARRAY_SIZE];            //      Buffer is needed for some functions
    bool run = true;                        //      Run controls main loop  //  run == false means
    bool read = true;                       //      If read == false, loop does not read a new line

    char *delim = check_and_set_delim(argv);        //      Array of possible delimeters
    int delim_counter;                              //      Counter of delimeters in current string
    read = count_delims(string, delim, &delim_counter);

    int line_number = 1;                        //      Current line number

    while (run)
    {
        read = check_arguments_part1(string, argc, argv,
                                     delim, &line_number, &delim_counter,
                                     read);
        if (read)
        {

            run = read_str(string, delim, &delim_counter);


        }

        if (check_arguments_part2(string, argc, argv,
                                  buffer, delim, &delim_counter,
                                  run, &line_number) == 1) return 0;

        line_number++;
        read = true;
    }

    return 0;
}

//  The function checks for the presence of "irow" and returns false if there is no need to read the next line
bool check_arguments_part1(char *string, int argc, char **argv,
                           char *delim, int *line_number, int *delim_number,
                           bool read)
{

    bool first_argument_exist;
    bool second_argument_exist;

    if  (argc <= MIN_ARGUMENTS)
    {
        return true;
    }
    else
    {
        for (int i = 1; i < argc; i++)          //      Loop goes through all arguments
        {

            first_argument_exist = argv[i + 1] != NULL;
            second_argument_exist = argv[i + 2] != NULL;

            if ((strcmp(argv[i], "irow") == 0) && (first_argument_exist))
            {

                int c = atoi(argv[i + 1]);

                if ((c > 0) && (*line_number == c))
                {

                    create_str(string, delim, delim_number);
                    return false;

                }
                continue;

            }

        }

    }

    if (read == false)
    {
        return false;
    }
    else
        return true;
}

//  The function checks for function calls and returns false if there is no need to read the next line
int check_arguments_part2(char *string, int argc, char **argv, char *buffer, char *delim, int *delim_number, bool run,
                          int *line_number)
{

    bool first_argument_exist;
    bool second_argument_exist;
    bool write = true;

    if  (argc <= MIN_ARGUMENTS)
    {
        return 0;
    }
    else
    {
        for (int i = 1; i < argc; i++)      //      Loop goes through all arguments
        {

            first_argument_exist = argv[i+1] != NULL;
            second_argument_exist = argv[i+2] != NULL;

            if ((strcmp(argv[i],"dcols")==0) && (first_argument_exist) && (second_argument_exist))      // Checks for function "dcols"
            {

                int n = atoi(argv[i + 1]);
                int m = atoi(argv[i + 2]);
                if ((n > m) || (n < 1)) continue;

                while (m >= n)
                {

                    char arg[10];
                    itoa(m, arg, 10);
                    dcol(string, buffer, delim, arg, delim_number);
                    m--;

                }

                continue;

            }
            else if ((strcmp(argv[i],"drow")==0) && (first_argument_exist))           // Checks for function "drow"
            {

                int r = atoi(argv[i+1]);
                if (r < 1) continue;
                if (r == *line_number)
                {

                    write = false;
                    break;

                }

            }
            else if ((strcmp(argv[i],"drows")==0) && (first_argument_exist) && (second_argument_exist))           // Checks for function "drows"
            {

                int n = atoi(argv[i+1]);
                int m = atoi(argv[i+2]);
                if ((n > m) || (n < 1)) continue;
                if ((*line_number >= n) && (*line_number <= m))
                {

                    write = false;
                    break;

                }

            }
            else if(strcmp(argv[i],"arow")==0)          //      Checks for function "arow"
            {

                if (run == false)
                {

                    write_in_file(string);
                    arow(string, delim, delim_number, run);

                }
                continue;

            }
            else if(strcmp(argv[i],"acol")==0)          //      Checks for function "acol"
            {

                if (acol(string, delim, delim_number)==1) {
                    return 1;
                }else
                    continue;

            }
            else if ((strcmp(argv[i],"rows")==0) && (first_argument_exist) && (second_argument_exist))          //      Checks for function "rows"
            {

                if ((strcmp(argv[i + 1], "-") == 0) && (strcmp(argv[i + 2], "-") == 0))
                {

                    if (run == false)
                    {
                        continue;
                    }
                    else
                        break;

                }
                else if ((strcmp(argv[i + 1], "-") != 0) && (strcmp(argv[i + 2], "-") == 0))
                {

                    if (*line_number >= atoi(argv[i + 1]))
                    {
                        continue;
                    }
                    else
                        break;

                }
                else if ((int) atoi(argv[i + 1]) <= atoi(argv[i + 2]))
                {

                    if ((*line_number >= (int) atoi(argv[i + 1])) &&
                        (*line_number <= (int) atoi(argv[i + 2])))
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }

                }
                else
                {
                    continue;
                }

            }
            else if((strcmp(argv[i],"beginwith") == 0) && (first_argument_exist) && (second_argument_exist))          //      Checks for function "beginwith"
            {

                if (beginwith(string, buffer, delim, argv[i + 1], argv[i + 2]))
                {
                    continue;
                }
                else
                    break;

            }
            else if((strcmp(argv[i],"contains") == 0) && (first_argument_exist) && (second_argument_exist))          //      Checks for function "contains"
            {

                if (contains(string, buffer, delim, argv[i + 1], argv[i + 2]))
                {
                    continue;
                }
                else
                    break;

            }
            else if((strcmp(argv[i],"csum")==0) && (first_argument_exist) && (argv[i+3] != NULL))          //      Checks for function "csum"
            {
                if (csum(string, buffer, delim, delim_number, argv[i + 1], argv[i + 2], argv[i + 3]) == 1)
                {
                    return 1;
                }
                else
                    continue;
            }
            else if((strcmp(argv[i],"cavg")==0) && (first_argument_exist) && (argv[i+3] != NULL))          //      Checks for function "cavg"
            {
                cavg(string, buffer, delim, delim_number, argv[i + 1],argv[i+2],argv[i+3]);
                continue;
            }
            else if((strcmp(argv[i],"cmin")==0) && (first_argument_exist) && (argv[i+3] != NULL))          //      Checks for function "cmin"
            {
                cmin(string, buffer, delim, delim_number, argv[i + 1],argv[i+2],argv[i+3]);
                continue;
            }
            else if((strcmp(argv[i],"cmax")==0) && (first_argument_exist) && (argv[i+3] != NULL))          //      Checks for function "cmax"
            {
                cmax(string, buffer, delim, delim_number, argv[i + 1],argv[i+2],argv[i+3]);
                continue;
            }
            else if((strcmp(argv[i],"icol")==0) && (first_argument_exist))          //      Checks for function "icol"
            {
                if(icol(string, buffer, delim, delim_number, argv[i + 1])==1) return 1;
                continue;
            }
            else if ((strcmp(argv[i],"dcol")==0) && (first_argument_exist))          //      Checks for function "dcol"
            {
                dcol(string, buffer, delim, argv[i + 1], delim_number);
                continue;
            }
            else if((strcmp(argv[i],"tolower") == 0) && (first_argument_exist))          //      Checks for function "tolower"
            {
                to_lower(string, delim, argv[i + 1]);
                continue;
            }
            else if((strcmp(argv[i],"toupper") == 0) && (first_argument_exist))          //      Checks for function "toupper"
            {
                to_upper(string, delim, argv[i + 1]);
                continue;
            }
            else if((strcmp(argv[i],"round") == 0) && (first_argument_exist))          //      Checks for function "round"
            {
                my_round(string, buffer, delim, delim_number, argv[i + 1]);
                continue;
            }
            else if((strcmp(argv[i],"int") == 0) && (first_argument_exist))          //      Checks for function "int"
            {
                my_int(string, buffer, delim, delim_number, argv[i + 1]);
                continue;
            }
            else if((strcmp(argv[i],"copy") == 0) && (first_argument_exist) && (second_argument_exist))          //      Checks for function "copy"
            {
                if (copy(string, buffer, delim, delim_number, argv[i + 1], argv[i + 2]) == 1)
                {
                    return 1;
                }
                else
                    continue;
            }
            else if((strcmp(argv[i],"swap") == 0) && (first_argument_exist) && (second_argument_exist))          //      Checks for function "swap"
            {
                swap(string, buffer, delim, delim_number, argv[i + 1], argv[i + 2]);
                continue;
            }
            else if((strcmp(argv[i],"move") == 0) && (first_argument_exist) && (second_argument_exist))          //      Checks for function "move"
            {
                move(string, buffer, delim, delim_number, argv[i + 1], argv[i + 2]);
                continue;
            }
            else if((strcmp(argv[i],"cset")==0) && (first_argument_exist) && (second_argument_exist))          //      Checks for function "cset"
            {
                if  (cset(string, buffer, delim, argv[i + 1], argv[i + 2], delim_number)==1)
                {
                    return 1;
                }
                else{
                    continue;
                }

            }

        }

        if (write)
        {
            write_in_file(string);
            write = true;
        }

    }
    return 0;
}

// Method finds content in C cell and puts it in the buffer;
void find_cell_content(char *string, char *buffer, char *delim, char *c) {

    int user_counter = atoi(c);
    int cell_counter = 1;

    int i = 0, j = 0;

    while (string[i] != '\n' && string[i] != '\0')
    {

        if (string[i] == delim[0])
        {
            cell_counter++;
            i++;
            continue;
        }

        if (cell_counter == user_counter)
        {
            buffer[j++] = string[i];
        }

        i++;

    }

    buffer[j] = '\0';

}

//  Finds the maximum value between N and M and assigns it to C
void cmax(char *string, char *buffer, char *delim, int *delim_number, char *c, char *n, char *m) {

    int user_counter = atoi(c);
    int n_in_int = atoi(n);
    int m_in_int = atoi(m);
    char tmp[101];
    double result = 0;
    int i = 0;
    bool is_number = true;
    bool is_first_cell = true;

    if ((user_counter < 1) || (n_in_int < 1) || (m_in_int < 1)) return;
    if ((n_in_int > m_in_int) || (n_in_int == m_in_int && m_in_int == user_counter)) return;
    if ((user_counter > *delim_number + 1) || (n_in_int > *delim_number + 1) || (m_in_int > *delim_number + 1)) return;
    if ((user_counter >= n_in_int) && (user_counter <= m_in_int)) return;

    while (m_in_int >= n_in_int)
    {
        find_cell_content(string,buffer,delim,m);
        while (buffer[i] != '\0')
        {

            if ((buffer[i] < '0') || (buffer[i] > '9') || (buffer[i] == '.'))
            {

                is_number = false;
                break;

            }
            i++;

        }

        if (!is_number)
        {
            m_in_int--;
            continue;
        }

        if (is_first_cell)
        {
            result = atof(buffer);
            is_first_cell = false;
            sprintf(tmp,"%i",m_in_int-1);
            m = tmp;
            m_in_int--;
            continue;
        }

        if  (result < atof(buffer)){

            result = atof(buffer);

            sprintf(tmp,"%i",m_in_int-1);
            m = tmp;
            m_in_int--;

        }
        else
        {
            m_in_int--;
            continue;
        }

    }
    memset(tmp,0,101);

    if ((int) result % 1 == 0)
    {
        sprintf(tmp, "%.f", result);
    }
    else
    {
        sprintf(tmp, "%.2f", result);
    }
    if (cset(string, buffer, delim, c,tmp, delim_number) == 1) return;

}

//  Finds the minimal value between N and M and assigns it to C
void cmin(char *string, char *buffer, char *delim, int *delim_number, char *c, char *n, char *m) {

    int user_counter = atoi(c);
    int n_in_int = atoi(n);
    int m_in_int = atoi(m);
    char tmp[101];
    double result = 0;
    int i = 0;
    bool is_number = true;
    bool is_first_cell = true;

    if ((user_counter < 1) || (n_in_int < 1) || (m_in_int < 1)) return;
    if ((n_in_int > m_in_int) || (n_in_int == m_in_int && m_in_int == user_counter)) return;
    if ((user_counter > *delim_number + 1) || (n_in_int > *delim_number + 1) || (m_in_int > *delim_number + 1)) return;
    if ((user_counter >= n_in_int) && (user_counter <= m_in_int)) return;

    while (m_in_int >= n_in_int)
    {
        find_cell_content(string,buffer,delim,m);
        while (buffer[i] != '\0')
        {

            if ((buffer[i] < '0') || (buffer[i] > '9') || (buffer[i] == '.'))
            {

                is_number = false;
                break;

            }
            i++;

        }

        if (!is_number)
        {
            m_in_int--;
            continue;
        }

        if (is_first_cell)
        {
            result = atof(buffer);
            is_first_cell = false;
            sprintf(tmp,"%i",m_in_int-1);
            m = tmp;
            m_in_int--;
            continue;
        }

        if  (result > atof(buffer)){

            result = atof(buffer);

            sprintf(tmp,"%i",m_in_int-1);
            m = tmp;
            m_in_int--;

        }
        else
        {
            m_in_int--;
            continue;
        }

    }
    memset(tmp,0,101);

    if ((int) result % 1 == 0)
    {
        sprintf(tmp, "%.f", result);
    }
    else
    {
        sprintf(tmp, "%.2f", result);
    }
    if (cset(string, buffer, delim, c,tmp, delim_number) == 1) return;

}

//  Finds the average value between N and M and assigns it to C
void cavg(char *string, char *buffer, char *delim, int *delim_number, char *c, char *n, char *m) {

    int user_counter = atoi(c);
    int n_in_int = atoi(n);
    int m_in_int = atoi(m);
    char tmp[101];
    double sum = 0;
    int counter_of_cell = 0;

    if ((user_counter < 1) || (n_in_int < 1) || (m_in_int < 1)) return;
    if ((n_in_int > m_in_int) || (n_in_int == m_in_int && m_in_int == user_counter)) return;
    if ((user_counter > *delim_number + 1) || (n_in_int > *delim_number + 1) || (m_in_int > *delim_number + 1)) return;
    if ((user_counter >= n_in_int) && (user_counter <= m_in_int)) return;

    while (m_in_int >= n_in_int) {

        find_cell_content(string,buffer,delim,m);
        sum += atof(buffer);
        sprintf(tmp,"%i",m_in_int-1);
        m = tmp;
        m_in_int--;
        counter_of_cell++;

    }
    memset(tmp,0,101);

    sprintf(tmp,"%.2f", sum / counter_of_cell);
    if (cset(string, buffer, delim, c, tmp, delim_number)==1) return;

}

//  Sums all cells between N and M and assigns it to C
int csum(char *string, char *buffer, char *delim, int *delim_number, char *c, char *n, char *m) {

    int user_counter = atoi(c);
    int n_in_int = atoi(n);
    int m_in_int = atoi(m);
    char tmp[101];
    double sum = 0;

    if ((user_counter < 1) || (n_in_int < 1) || (m_in_int < 1)) return 0;
    if ((n_in_int > m_in_int) || (n_in_int == m_in_int && m_in_int == user_counter)) return 0;
    if ((user_counter > *delim_number + 1) || (n_in_int > *delim_number + 1) || (m_in_int > *delim_number + 1)) return 0;
    if ((user_counter >= n_in_int) && (user_counter <= m_in_int)) return 0;

    while (m_in_int >= n_in_int) {

        find_cell_content(string,buffer,delim,m);
        sum += atof(buffer);
        sprintf(tmp,"%i",m_in_int-1);
        m = tmp;
        m_in_int--;

    }
    memset(tmp,0,101);

    if ((int) sum % 1 == 0)
    {
        sprintf(tmp,"%.f",sum);
    }
    else
    {
        sprintf(tmp,"%.2f",sum);
    }
    if (cset(string, buffer, delim, c, tmp, delim_number)==1) return 1;
    return 0;
}

//  The processor will process only those rows whose cells in column C contain the string STR
bool contains(char *string, char *buffer, char *delim,
              char *c, char *STR)
{

    int user_counter = atoi(c);
    if (user_counter < 1) return true;
    int cell_counter = 1;

    find_cell_content(string,buffer,delim,c);

    if (strstr(buffer,STR) != NULL)
    {
        return true;
    }

    return false;
}

//  The processor will process only those rows whose cell contents in column C begin with a string STR
bool beginwith(char *string, char *buffer, char *delim,
               char *c, char *STR)
{

    int user_counter = atoi(c);
    if (user_counter < 1) return true;
    int k = 0;
    int cell_counter = 1;

    find_cell_content(string,buffer,delim,c);

    while (k < strlen(STR) && buffer[k] != '\0')
    {

        if (buffer[k] != STR[k])
        {
            return false;
        }
        k++;

    }

    return true;
}

//  Moves column N before column M
void move(char *string, char *buffer, char *delim, int *delim_number, char *n, char *m)
{

    int n_in_int = atoi(n);
    int m_in_int = atoi(m);
    if (m_in_int > *delim_number + 1) return;
    if (n_in_int < 1) return;


    if (icol(string, buffer, delim, delim_number, m) == 1) return;

    memset(buffer,0,MAX_ARRAY_SIZE);

    if ((m-n) == 1)
    {
        return;
    }
    else if (n_in_int > m_in_int)
    {

        char n_back_in_string[6];
        char m_back_in_string[6];

        sprintf(n_back_in_string, "%d", n_in_int + 1);
        sprintf(m_back_in_string, "%d", m_in_int + 1);

        if (copy(string, buffer, delim, delim_number, n_back_in_string, m) == 1) return;
        memset(buffer,0,MAX_ARRAY_SIZE);
        dcol(string, buffer, delim, n_back_in_string, delim_number);
        memset(buffer,0,MAX_ARRAY_SIZE);

    }
    else if (n_in_int < m_in_int)
    {

        if (copy(string, buffer, delim, delim_number, n, m) == 1) return;
        memset(buffer,0,MAX_ARRAY_SIZE);
        dcol(string, buffer, delim, n, delim_number);
        memset(buffer,0,MAX_ARRAY_SIZE);

    }
    else
        return;

}

// Swaps the values of the cells in columns N and M
void swap(char *string, char *buffer, char *delim, int *delim_number, char *n, char *m)
{

    int n_in_int = atoi(n);
    int user_counter = atoi(m);

    if (n_in_int < 1) return;
    if (user_counter < 1) return;
    if (n_in_int == user_counter) return;
    if (user_counter > *delim_number + 1) return;

    int k = 0;
    int cell_counter = 1;
    char result_from_m[MAX_IN_BOX];

    find_cell_content(string,buffer,delim,m);


    while (buffer[k] != '\0' && k < MAX_IN_BOX)
    {

        result_from_m[k] = buffer[k];
        k++;

    }
    result_from_m[k] = '\0';
    memset(buffer,0,MAX_ARRAY_SIZE);

    if (copy(string, buffer, delim, delim_number, n, m) == 1) return;
    if (cset(string, buffer, delim, n, result_from_m, delim_number) == 1) return;

}

//  Copies the values of the cells in columns N and M
int copy(char *string, char *buffer, char *delim, int *delim_number, char *n, char *m)
{

    char result[MAX_IN_BOX];
    int k = 0;
    int cell_counter = 1;

    int user_counter = atoi(n);
    int m_in_int = atoi(m);

    if (user_counter < 1) return 0;
    if (m_in_int < 1) return 0;
    if (m_in_int == user_counter) return 0;
    if (m_in_int > *delim_number + 1) return 0;

    find_cell_content(string,buffer,delim,n);

    while (buffer[k] != '\0' && k < MAX_IN_BOX)
    {

        result[k] = buffer[k];
        k++;

    }
    result[k] = '\0';
    memset(buffer,0,MAX_ARRAY_SIZE);
    if (cset(string, buffer, delim, m, result, delim_number)==1) return 1;
    return 0;
}

//  Removes the decimal part of the number in column C
void my_int(char *string, char *buffer, char *delim, int *delim_number, char *c)
{

    int i = 0, j = 0;
    int cell_counter = 1;

    int user_counter = atoi(c);
    if (user_counter < 1) return;

    while (string[i] != '\n' && string[i] != '\0')
    {

        if (string[i] == delim[0])
        {
            cell_counter++;
            i++;
            continue;
        }

        if (cell_counter == user_counter)
        {

            if ((string[i] >= 48) && (string[i] <= 57) || (string[i] == 46))
            {
                buffer[j++] = string[i];
            }
            else
            {
                memset(buffer,0,MAX_ARRAY_SIZE);
                return;
            }

        }
        i++;

    }
    buffer[j] = '\0';

    char result_in_str[MAX_IN_BOX];
    sprintf(result_in_str,"%i",(int) atof(buffer));
    memset(buffer,0,MAX_ARRAY_SIZE);
    if (cset(string, buffer, delim, c, result_in_str, delim_number)==1) return;

}

//  In column C, rounds the number to an integer
void my_round(char *string, char *buffer, char *delim,
              int *delim_number, char *c)
{

    int user_counter = atoi(c);
    if (user_counter < 1) return;
    int i = 0, j = 0;
    int cell_counter = 1;
    char result_in_str[MAX_IN_BOX];

    while (string[i] != '\n' && string[i] != '\0')
    {

        if (string[i] == delim[0])
        {
            cell_counter++;
            i++;
            continue;
        }

        if (cell_counter == user_counter)
        {
            if ((string[i] >= 48) && (string[i] <= 57) || (string[i] == 46))
            {
                buffer[j++] = string[i];
            }
            else
            {
                memset(buffer,0,MAX_ARRAY_SIZE);
                return;
            }

        }

        i++;

    }
    buffer[j] = '\0';

    sprintf(result_in_str,"%.f",atof(buffer));
    memset(buffer,0,MAX_ARRAY_SIZE);
    if (cset(string, buffer, delim, c, result_in_str, delim_number)==1) return;

}

//  The string in column C will be converted to uppercase
void to_upper(char *string, char *delim, char *c)
{

    int user_counter = atoi(c);
    if (user_counter < 1) return;
    int i = 0;
    int cell_counter = 1;

    while (string[i] != '\n' && string[i] != '\0')
    {

        if (string[i] == delim[0])
        {
            cell_counter++;
            i++;
            continue;
        }

        if (cell_counter == user_counter)
        {
            if (string[i] >= 97 && string[i] <= 122)
            {
                string[i] = string[i] - 32;
            }
        }

        i++;

    }

}

//  The string in column C will be converted to lowercase
void to_lower(char *string, char *delim, char *c)
{

    int user_counter = atoi(c);
    if (user_counter < 1) return;
    int i = 0;
    int cell_counter = 1;

    while (string[i] != '\n' && string[i] != '\0')
    {

        if (string[i] == delim[0])
        {
            cell_counter++;
            i++;
            continue;
        }

        if (cell_counter == user_counter)
        {
            if ((string[i] >= 65) && (string[i] <= 90))
            {
                string[i] = string[i] + 32;
            }

        }

        i++;

    }

}

//  The string STR will be set to the cell in column C.
int cset(char *string, char *buffer, char *delim, char *c, char *STR, int *delim_number)
{

    int cell_counter = 1;
    int user_counter = atoi(c);
    if (user_counter < 1) return 0;
    int size = strlen(string);
    int index = 0;
    int j = 0, k = 0;

    if (size + strlen(STR) >= MAX_ARRAY_SIZE || strlen(STR) >= 99)
    {
        fprintf(stderr,"error: STR is too big.\n");
        return 1;
    }

    while (true)
    {

        for (int i = index; i < size; ++i)
        {

            if (string[i] == delim[0])
            {
                index = i;
                break;
            }

        }
        if (cell_counter == user_counter)
        {

            int i = 0;
            while (i < strlen(STR))
            {
                buffer[j] = STR[i];
                j++;
                i++;
            }

            k = index;
            if (cell_counter != (*delim_number + 1))
            {

                while (k < size)
                {

                    buffer[j] = string[k];
                    j++;
                    k++;

                }
            }

            else if (string[size - 1] == '\n')
            {
                buffer[j++] = '\n';
            }
            break;

        }
        else
        {
            while (j <= index)
            {
                buffer[j] = string[k];
                j++;
                k++;
            }

            cell_counter++;
            index++;

        }

    }

    buffer[j] = '\0';
    strcpy(string, buffer);
    memset(buffer,0,MAX_ARRAY_SIZE);
    return 0;

}

//  Adds a new table row to the end of the table
void arow(char *string, char *delim, int *delim_number, bool run)
{

    if (run == false)
    {

        putc('\n',stdout);
        create_str(string, delim, delim_number);
        string[strlen(string) - 1] = '\0';
        run = true;

    }

}

//  Creates string of delims        Example: "::::::"
void create_str(char *string, char *delim, int* line_number)
{

    int i = 0;
    while (i < *line_number)
    {
        string[i++] = delim[0];
    }

    string[i++] = '\n';
    string[i] = '\0';

}

//  Counts delimeters in string
bool count_delims(char *string, char *delim, int *delim_number)
{

    int c;
    int k = 0;
    bool read = true;
    *delim_number = 0;
    c = getc(stdin);

    while ((c != '\n'))
    {

        if (c == EOF)
        {
            read = false;
            break;
        }

        for (int i = 0; i < strlen(delim); ++i)
        {

            if (c == delim[i])
            {
                c = delim[0];
                *delim_number += 1;
            }

        }

        if (k >= MAX_ARRAY_SIZE-1)
        {
            memset(string,0,MAX_ARRAY_SIZE);
            fprintf(stderr,"error: input is too big!\n");
            return false;
        }

        string[k++] = c;

        c = getc(stdin);

    }

    if  (read)
    {
        string[k] = '\n';
    }

    if (read)
    {
        while (k >= 0)
        {
            ungetc(string[k--], stdin);
        }

    }

    return read;

}

// Checks the existence of the "-d" parameter and assigns a divisor
char* check_and_set_delim(char *argv[])
{

    if  ((argv[1] != NULL) && (strcmp(argv[1],"-d")==0) && (argv[2] != NULL))
    {
        return argv[2];
    }
    else
    {
        return " ";
    }

}

//  Inserts an empty column before the column given by the number C
int icol(char *string, char *buffer, char *delim,
         int *delim_number, char *c)
{

    int cell_counter = 1;
    int user_counter = atoi(c);
    if (user_counter < 1) return 0;
    bool delim_set = false;
    int i = 0, j = 0;
    int size = strlen(string);

    if (size >= MAX_ARRAY_SIZE-1){

        fprintf(stderr,"error: array is too big.\n");
        return 1;

    }

    while(i < size)
    {

        if  ((string[i] == delim[0]) && (cell_counter == user_counter) && (!delim_set))
        {

            buffer[j++] = delim[0];
            delim_set = true;
            cell_counter++;
            continue;

        }

        if  (string[i] == delim[0])
        {
            cell_counter++;
        }

        if  ((cell_counter == user_counter) && (!delim_set))
        {

            buffer[j++] = delim[0];
            delim_set = true;
            continue;

        }

        buffer[j++] = string[i++];

    }

    buffer[j] = '\0';
    strcpy(string, buffer);
    *delim_number += 1;

    return 0;
}

//  Deletes the column given by the number C
void dcol(char *string, char *buffer, char *delim,
          char *c, int *delim_number)
{

    int cell_counter = 1;
    int user_counter = atoi(c);
    if (user_counter < 1) return;
    int i = 0, j = 0;
    while (string[i] != '\0')
    {

        if (string[i] == delim[0])
        {

            if ((cell_counter == user_counter) && (user_counter == 1))
            {

                i++;
                cell_counter++;

            }

            cell_counter++;

            if (cell_counter == user_counter)
            {
                i++;
            }
            else
            {
                buffer[j++] = string[i++];
            }

        }
        else if ((cell_counter == user_counter) && (string[i] != '\n'))
        {
            i++;
        }
        else if (cell_counter == user_counter)
        {
            buffer[j++] = string[i++];
        }
        else
        {
            buffer[j++] = string[i++];
        }

    }

    buffer[j] = '\0';
    strcpy(string, buffer);
    *delim_number -= 1;

}

//  Adds an empty column after the last column
int acol(char *string, char *delim, int *delim_number)
{

    int size = strlen(string);

    char delimeter = delim[0];
    if (size >= MAX_ARRAY_SIZE-1) {

        memset(string,0,MAX_ARRAY_SIZE);
        fprintf(stderr,"error: input is too big.\n");
        return 1;

    }

    if ((string[size - 1]) == '\n')
    {

        string[size - 1] = '\0';
        sprintf(string, "%s%c%c%c", string, delimeter, '\n','\0');

    }
    else
    {
        sprintf(string, "%s%c%c", string, delimeter,'\0');
    }

    *delim_number += 1;

    return 0;
}

//  Writes current string in stdout
void write_in_file(char *string)
{

    for (int i = 0; string[i] != '\0'; ++i)
    {
        putc(string[i], stdout);
    }

}

//  Reads string from stdin
bool read_str(char *string, char *delim, int *delim_number)
{

    int c;
    int counter = 0;
    *delim_number = 0;
    while (true) {

        c = getc(stdin);
        for (int i = 0; i < strlen(delim); ++i)
        {

            if  (c == delim[i])
            {
                c = delim[0];              //      Cycle replaces divisors with the first
                *delim_number += 1;
            }

        }
        if (counter >= MAX_ARRAY_SIZE-1)
        {
            memset(string,0,MAX_ARRAY_SIZE);
            fprintf(stderr,"error: input is too big!\n");
            return false;
        }

        string[counter++] = c;

        if  (c == '\n')
        {
            string[counter] = '\0';
            return true;
        }
        else if (c == EOF)
        {
            counter--;
            string[counter] = '\0';
            return false;
        }

    }

}
#include <stdio.h>
#include <stdlib.h>

// Huiwen Zhou 2094248
// Man Luo 2076393
// Qianxin Zhou 2086858

int main()
{
    FILE *input_file = fopen("input.txt", "r");
    FILE *output_file = fopen("output.txt", "w");

    if (input_file == NULL || output_file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    int count = 0;
    int l = 0, r = 0, t = 0, b = 0;
    int x, y;
    // read the coordinates of each point before it finds 'E'
    // if wrong input, print error and break
    while (1)
    {
        int result = fscanf(input_file, "%d,%d", &x, &y);

        // chech if the next character is 'E'
        int next_char = fgetc(input_file);
        if (next_char == 'E' || next_char == EOF)
        {
            break;
        }
        else
        {
            // put the character back to input stream for the next read
            ungetc(next_char, input_file);
        }

        // check for missing coordinates
        if (result != 2)
        {
            // check if it's end of file 
            if (feof(input_file))
            {
                break;
            }
            else
            {
                // otherwise, print error and exit
                printf("Error: Missing x or y.\n");
                fprintf(output_file, "ERROR");
                return 1;
            }
        }

        if (x < 0 || y < 0)
        {
            // if got negative coordinates, print error and exit
            printf("Error: negative x or y.\n");
            fprintf(output_file, "ERROR");
            return 1;
        }

        // count the number of points read
        count++;
        // record the left, right, top and bottom bounds of the shape
        if (x < l)
        {
            l = x;
        }
        else if (x > r)
        {
            r = x;
        }
        if (y < b)
        {
            b = y;
        }
        else if (y > t)
        {
            t = y;
        }
    }

    // malloc for a 2D array accordingly
    int row = t - b + 1;
    int column = r - l + 1;
    // printf("row: %d coloum: %d\n", row, column);
    char **arr = (char **)malloc(row * sizeof(char *));
    if (arr == NULL)
    { // check if malloc successfully
        printf("malloc fail\n");
        return 1; 
    }

    for (int i = 0; i < row; i++)
    {
        arr[i] = (char *)malloc(column * sizeof(char));
        if (arr[i] == NULL)
        { 
            printf("malloc fail\n");

            // if fail, release the memory previously allocated
            for (int j = 0; j < i; j++)
            {
                free(arr[j]);
            }
            free(arr);
            return 1; 
        }
    }
    // initialize each element in the array to ' '
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            arr[i][j] = ' ';
        }
    }

    // reread input file and add points and lines to the 2D array
    fseek(input_file, 0, SEEK_SET); // Reset the file pointer to the beginning.
    int i = 0;
    int prev_x = -1;
    int prev_y = -1;
    int distant, sx, sy;
    while (i < count)
    {
        fscanf(input_file, "%d,%d", &x, &y);
        arr[row - y - 1][x] = '*';
        //  If it's not the first point, start drawing lines
        if (prev_x >= 0 && prev_y >= 0)
        {
            if (x - prev_x != 0)
            {
                distant = abs(x - prev_x);
            }
            else if (y - prev_y != 0)
            {
                distant = abs(y - prev_y);
            }
            // If the previous and current points coincide, report an error and exit
            else
            {
                printf("Error: 2 subsequent points coincide");
                fprintf(output_file, "ERROR");
                return 1;
            }
            sx = (x - prev_x) / distant;
            sy = (y - prev_y) / distant;
            int temp_x = prev_x, temp_y = prev_y;
            for (int i = 0; i < distant - 1; i++)
            {
                temp_x = temp_x + sx;
                temp_y = temp_y + sy;
                // printf("lines in between: (%d,%d) \n", temp_x, temp_y);
                arr[row - temp_y - 1][temp_x] = '*';
            }
        }
        // Once new points and lines added
        // store the coordinates of the new point in prev_x and prev_y for the next iteration
        prev_x = x;
        prev_y = y;
        i++;
    }

    // Start filling: scan each row from left to right, fill the positions between two '*'
    int flag, p1_y, p2_y;
    for (int i = 0; i < row; i++)
    {
        p1_y = 0;
        p2_y = 0;
        flag = 0; //flag to indicate whether * has been encountered before.
        for (int j = 0; j < column; j++)
        {
            if (arr[i][j] == '*')
            {
                if (flag)
                {
                    p2_y = j;
                    while (p1_y < p2_y - 1)
                    {
                        p1_y++;
                        arr[i][p1_y] = '*';
                        // printf("filling the point: (%d,%d) \n", i, p1_y);
                    }
                }
                else
                {
                    p1_y = j;
                    flag = 1;
                }
            }
        }
    }

    // print the 2D arry into output file
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            fprintf(output_file, "%c", arr[i][j]);
        }
        fprintf(output_file, "\n");
    }
    fclose(input_file);
    fclose(output_file);

    // release allocated memories
    for (int i = 0; i < row; i++)
    {
        free(arr[i]);
    }
    free(arr);
    return 0;
}
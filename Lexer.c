
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define tokenMax 100
char keyWords[100][50] = {"int","float","boolean","string","while","until","if else", "true", "false", "long","double","char","for","if","else","switch","break","continue","return"};
#define countK 19
#define keyRange 100
char operators[100][4] = { "+", "-", "*", "/", "%", ":=", "==", ">", "<", ">=", "<=", "!=", "&&", "||", "!", "?", ":", "="};
#define countO 18
#define opRange 110
char delimiters[100][4] = { "{", "}", "(", ")", "[", "]", ";", ","};
#define countD 8
#define delRange 126

FILE *file,*file1;
char buffer[100];
int flag = 0;
int countLine = 0;

int lexerDFA(char tk[100]){
    //First checking for error(invalid token),Keyword,Operator,Delimiter.
    //If not matching with either, then moving on to the dfa.
    int i;
    //Generating error for invalid token
    if(strlen(tk) == 0)
        return 0;
    //Found a keyword
    for(i=0;i<countK;i++){
        if(strcmp(tk,keyWords[i]) == 0){
            fprintf(file1, "Token %d, string(keyword) %s, line number %d\n",keyRange+i,tk,countLine);
            printf("Token %d, string(keyword) %s, line number %d\n",keyRange+i,tk,countLine);
            return 0;
        }
    }
    //Found an operator
    for(i=0;i<countO;i++){
        if(strcmp(tk,operators[i]) == 0){
            fprintf(file1, "Token %d, string(operator) %s, line number %d\n",opRange+i,tk,countLine);
            printf("Token %d, string(operator) %s, line number %d\n",opRange+i,tk,countLine);
            return 0;
        }
    }
    //Found a delimiter
    for(i=0;i<countD;i++){
        if(strcmp(tk,delimiters[i]) == 0){
            fprintf(file1, "Token %d, string(delimiter) %s, line number %d\n",delRange+i,tk,countLine);
            printf("Token %d, string(delimiter) %s, line number %d\n",delRange+i,tk,countLine);
            return 0;
        }
    }

    //DFA: Checking for Integer Literal, Floating Point Literal, String Literal. 
    // Starting State: State 1
    // Final or Accepted State: State 11 
    i = 0;
    int state = 1;
    char curr_tk;
    while(tk[i] != '\0' && state != 11)
	{
        curr_tk = tk[i++];
        switch(state)
		{
            case 1:
                if(isalpha(curr_tk))
                    state = 2; //identifier if first tk is a char
                else if(curr_tk == '+' || curr_tk == '-')
                    state = 3; //operator + - 
                else if(isdigit(curr_tk) && curr_tk != '0')
                    state = 4; //integer literal (but not zero)
                else if(curr_tk == '0')
                    state = 5; //integer literal zero
                else if(curr_tk == '"')
                    state = 6; //string literal
                else
                    state = 11; //final state -> will exit the loop
                break;
            case 2: 
                if(isalpha(curr_tk) || isdigit(curr_tk) || curr_tk == '_')
                    state = 2; //identifier 
                else
                    state = 11; //final state -> will exit the loop
                break;
            case 3:
                if(curr_tk != '0' && isdigit(curr_tk))
                    state = 4; //integer literal (but not zero)
                else if(curr_tk == '0')
                    state = 10; //looking for float
                else
                    state = 11; //final state -> will exit the loop
                break;
            case 4:
                if(isdigit(curr_tk))
                    state = 4; //integer literal (but not zero)
                else if(curr_tk == '.')
                    state = 7; //looking for float
                else 
                    state = 11; //final state -> will exit the loop
                break;
            case 5:
                if(curr_tk == '.')
                    state = 7; //looking for float
                else
                    state = 11; //final state -> will exit the loop
                break; 
            case 6:
                if(curr_tk != '"')
                    state = 6; //not a string literal
                else
                    state = 9; //string literal
                break;
            case 7:
                if(isdigit(curr_tk))
                    state = 8; //Floating Point Literal
                else 
                    state = 11; //final state -> will exit the loop
                break;
            case 8:
                if(isdigit(curr_tk))
                    state = 8; //Floating Point Literal
                else
                    state = 11; //final state -> will exit the loop
                break;
            case 9:
                state = 11; //final state -> will exit the loop
                break;
            case 10:
                if(curr_tk == '.')
                    state = 7; //Looking for float
                else
                    state = 11; //final state -> will exit the loop
            case 11:
                //generating error
                fprintf(file1, "invalid token\n");
                printf("invalid token\n");
                return 11; //final state -> will exit the loop
        }
    }
    //Printing based on state (case) of token
    switch(state){
        case 2:// Identifier
            fprintf(file1, "Token %d, string(id) %s, line number %d\n",132,tk,countLine);
            printf("Token %d, string(id) %s, line number %d\n",132,tk,countLine);
            break;
        case 4://Integer Literal
            fprintf(file1, "Token %d, string(integer) %s, line number %d\n",130,tk,countLine);
            printf("Token %d, string(integer) %s, line number %d\n",130,tk,countLine);
            break;
        case 5://Only for Integer Literal = 0
            fprintf(file1, "Token %d, string(integer) %s, line number %d\n",131,tk,countLine);
            printf("Token %d, string(integer) %s, line number %d\n",131,tk,countLine);
            break;
        case 8://Floating Point Literal
            fprintf(file1, "Token %d, string(float)%s, line number %d\n",133,tk,countLine);
            printf("Token %d, string(float) %s, line number %d\n",133,tk,countLine);
            break;
        case 9://String Literal
            fprintf(file1, "Token %d, string %s, line number %d\n",134,tk,countLine);
            printf("Token %d, string %s, line number %d\n",134,tk,countLine);
            break;
        default:
            fprintf(file1, "Token %d, invalid %s, line number %d\n",404,tk,countLine);
            printf("Token %d, invalid %s, line number %d.......\n",404,tk,countLine);
            break;
    }
    return state;
}

// Looking for operator/delimiter
int lookup_op_de(char a){   
    int i;
    for(i=0;i<countO;i++){
        if(a == operators[i][0])
            return 1;
    }
    for(i=0;i<countD;i++){	
        if(a == delimiters[i][0])
            return 2;
    }
    return 0;
}

//Looking for words. For Example: "Bitsian" "while" "PSH" 
int tokenizer(char tk[100]){    
    int checked,flag1=0,i=0,k=0;
    char buffer[100];
    while(tk[i] != '\0'){   
		//Check for operator and delimiter
        //checked == 0 (Didnt find operator or delimiter)
        //checked == 1 (Operator)
        //checked == 2 (Delimiter)
        checked = lookup_op_de(tk[i]);
        //When read Operator
        if(flag1 == 1){
			//After finding an operator, Looking for Boolean operator
            if(tk[i] == '=' || tk[i] == '&' || tk[i] == '|'){
                buffer[k++] = tk[i];
                flag1 = 2;
            }
            buffer[k] = '\0';
            lexerDFA(buffer);
            k=0;
            if(flag1 != 2){ //didnt find boolean operator
                buffer[k++] = tk[i];
            }
            flag1 = 0;
            i++;
            continue;
        }
        // Didn't find Operator or Delimiter (0)
        if(checked == 0)
            buffer[k++] = tk[i];
		// Found an operator (1)
        else if(checked == 1){
            flag1 = 1;
            buffer[k] = '\0';
            lexerDFA(buffer);
            k=0;
            buffer[k++] = tk[i];
        }
		// Found a delimiter (2)
        else if(checked == 2){
            buffer[k] = '\0';
            lexerDFA(buffer);
            k=0;
            buffer[k++] = tk[i];
            buffer[k] = '\0';
            lexerDFA(buffer);
            k=0;
        }
        i++;
    }
    //tk found to be '\0' but k!=0 (buffer not empty)
    if(k!=0){
        buffer[k] = '\0';
        lexerDFA(buffer);
    }
}

int tkLexer(char line[100]){
    int i,x,k=0;
    for(i=0;i<100;i++)
        buffer[i]=0;
    for(i=0;i<strlen(line);i++){
		// Looking for Comment
        if(line[i] == '/' && line[i+1] == '/' && i+1 < strlen(line) && flag == 0){
            flag=2;
            i+=2;
        } // Looking for newline after Comment
        if(line[i] == '\n' && flag == 2 ){	
            flag=0; //reset flag to 0
            i+=1;
        } // Looking for Multi Line Comment 
        if(line[i] == '/' && line[i+1] == '*' && i+1 < strlen(line) && flag == 0){
            flag=1;
            i+=2;
        }// Looking for Multi Line Comment End
        if(line[i] == '*' && line[i+1] == '/' && i+1 < strlen(line) && flag == 1 ){
            flag=0; //reset flag to 0
            i+=2;
        }
		// Looking for Regular Lines (Not within comments)
        else if(flag == 0){
			// Looking for new line, space or eof(end of file)
            if((line[i] == '\n'|| line[i] == ' ' || line[i] == '\0') && k!=0){
                buffer[k] = '\0';
                tokenizer(buffer); //call to tokenizer
                k=0;
            }
            else{
                if(line[i] != '\n'|| line[i] != ' ' || line[i] != '\0')
                    buffer[k++] = line[i];
            }
        }
   }
}

int main ( void ){
    char filename[250],filename1[250];    
    scanf("%s",filename);
    scanf("%s",filename1);
    file = fopen ( filename, "r" );
    file1 = fopen ( filename1, "w" );
    if ( file != NULL ){
        char line [100]; 
        while ( fgets ( line, sizeof line, file ) != NULL ) {
			++countLine;
            tkLexer(line);
        }
        fclose ( file );
        fclose ( file1 );
   	}
   else{
      //generating error
      printf("unable to open file\n"); 
   }
   return 0;
}
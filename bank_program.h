#ifndef FINAL_ASSIGNMENT_BANK_PROGRAM_H
#define FINAL_ASSIGNMENT_BANK_PROGRAM_H

#include "stdio.h"
#include "AVL.h"
#include "time.h"
#define SIZE 10000

struct Seed db[SIZE];
struct Node *root=NULL;

struct my_time{
    char c_time[25];
};

struct my_time Ctime[1];

int users=0;
int userIndex=0;
int user_option=0;
int dataType_Valid=0;
int existence=-1;
int gmailValid=-1;
int emailExist=-1;
int phone_valid=-1;
int phone_found=-1;
int nrc_valid =-1;
int strongPass=-1;
int two_charArray=-1;
int porB=-1;
int transaction_valid=-1;
int cashOut_limit=-1;
int option_key=0;

unsigned int trans_limit=0;
unsigned int currentDay=0;
unsigned int total_currentAmt_transfer=0;
unsigned int total_currentAmt_receive=0;
unsigned total_CashOut=0;

char int_to_char_array_data[10];
char double_to_char_data[10];

double annual_InterestRate=7.0; //fixed rate
double monthly_InterestRate=7.0/ 12;
double tolerance = 0.09;
int total_interestRate_Minute=0;
int cur_interest_rateMinute=0;
int started_interest_rateMinute=0;
int loanOrRepay=-1;

char str_input[1024];

void welcome(int client_socket);
void loading_data();
void printing_data();
void signUp(int client_socket);
void logIN(int client_socket);
void user_sector(int client_socket, int index);

void trim(char *str);
void emailExistChecking(struct Node *root,char toCheck[50]);
void gmailValidation(char mailToValidate[50]);
void nrcValidation(char nrc_toCheck[50]);
void int_checker(char input[]);
void phone_validation(struct Node *root,unsigned long long int phone);
void pOrb_validation(int i);
void passwordExistChecking(struct Node *root,char pass[50]);
void strongPassword(char pass[50]);
void searching(struct Node *root,unsigned int data);
void copy_two_char_array(char receiver[200],char transmitter[200]);
void compare_two_charArray(char first[50], char second[50]);
int charCounting(char toCount[50]);

void transfer(int index, int client_socket);
void calculation_transAmt_on_sameDay(unsigned int amount, int client_socket);
void transfer_money(int transfer,int receiver ,unsigned int amount, int client_socket);
void transaction_record(int transfer, int receiver, char who, unsigned int amount);
void calculate_withdrawAmt_sameDay(int index, unsigned int amount, int client_socket);
void with_draw(int index, int client_socket);
void withdraw_record(int index,unsigned int cur_amt,unsigned int withdrawAmt);
void cashIn(int index, int client_socket);
void cashIn_record(int index, unsigned int cur_amt, unsigned int cashInAmt);

void loan(int index, int client_socket);
void current_interest_rate_time(int index);
void started_interest_rate_time(int index);
void current_payment(int index);
void proceed_loan(int index, double new_loanAmt, int client_socket);
void loanTime_record(int index);
void loan_record(int index, double loanAmount, double total_loan_payment);
void repayment(int index, int client_socket);
void repayment_record(int index, double repaid_amount, double remaining_payment);

void get_limit_amount(int index);
void get_time();
void get_current_day();
void integer_to_char(unsigned int value);
unsigned int charArray_to_unsigned_fun(char charArray[]);
int charArray_to_int(char charArray[]);
int charArray_to_llu(char charArray[]);
void double_to_char(double value);
double double_var_checking(double value);

void update_information(int index, int client_socket);
void upgrade_accLvl_record(int index);
void history(int index, int client_socket);
void back_option(int client_socket, int index);
void proceed_or_back(int client_socket, int index);

void print_t_file(struct Node *root);
void print_trackData_t_file(struct Node *root);
void print_loanData_t_file(struct Node *root);
void recordData_to_file(struct Node *root);

void welcome(int client_socket){

    const char *string1 = "**>> Welcome to our Bank <<**";
    const char *string2 = "Press 1 to Login:";
    const char *string3 = "Press 2 to Sign Up:";
    const char *string4 = "Press 3 to Exit:";

    char message[4096];
    snprintf(message, sizeof(message), "\n%s\n%s\n%s\n%s\n", string1, string2, string3, string4);

    int bytes_sent = send(client_socket, message, strlen(message), 0);
    if (bytes_sent == -1) {
        fprintf(stderr, "send() failed. (%d)\n", GETSOCKETERRNO());
        CLOSESOCKET(client_socket);
    }

}

void loading_data(){

    FILE *fptr2=fopen("bankInfo_tracking.txt","r");
    if(fptr2==NULL){
        printf("ERROR IN LOADING!!");
    }else{
        for(int adjust=0; adjust<SIZE; adjust++){
            fscanf(fptr2,"%d%u%u%u%u%u%d%d",
                   &db[adjust].tracking.recordCounter ,&db[adjust].tracking.totalTransfer
                    ,&db[adjust].tracking.totalReceive ,&db[adjust].tracking.totalCashOut
                    ,&db[adjust].tracking.trans_perDay ,&db[adjust].tracking.cashOut_perDay
                    ,&db[adjust].tracking.loanTime_rec_counter ,&db[adjust].tracking.repaid_switch);

            if(db[adjust].tracking.trans_perDay == ' '){
                break;
            }
        }
    }

    FILE *fptr=fopen("bankData.txt","r");
    if(fptr==NULL){
        printf("ERROR IN LOADING!!");
    }else{
        for(int adjust=0; adjust<SIZE; adjust++){
            fscanf(fptr,"%u%s%s%llu%s%s%s%s%u%lf%u%d",
                   &db[adjust].id ,&db[adjust].name ,&db[adjust].email ,&db[adjust].phNumber ,
                   &db[adjust].nrc ,&db[adjust].address ,&db[adjust].password ,&db[adjust].pOrb ,
                   &db[adjust].cur_amount ,&db[adjust].total_loanAmount ,&db[adjust].transAmoLimitPerDay ,&db[adjust].acc_level);

            for(int gcc=0; gcc<db[adjust].tracking.recordCounter; gcc++){
                fscanf(fptr,"%s",&db[adjust].record[gcc].note);
            }

            if(db[adjust].email[0] == '\0'){
                break;
            }

            root=insertNode(root,&db[adjust]);

            users++;
            db[users].index=users;
        }
    }

    FILE *fptr3=fopen("loanPayment_Record.txt","r");
    if(fptr3==NULL){
        printf("ERROR IN LOADING!!");
    }else{
        for(int adjust=0; adjust<SIZE; adjust++){
            fscanf(fptr3,"%lf",&db[adjust].tracking.loan_payment);
            fscanf(fptr3,"%lf",&db[adjust].tracking.total_loan_payment);
            fscanf(fptr3,"%s",&db[adjust].tracking.loanTime_Rec);
            fscanf(fptr3,"%s",&db[adjust].tracking.repaidTime_Rec);

            if(adjust == users){
                fclose(fptr3);
                break;
            }
        }
    }

    fclose(fptr2);
    fclose(fptr);

}

void printing_data(){

    /*preOrder_track(root);
    printf("\n");
    preOrder_loan(root);*/

    // preOrder(root);
    inOrder(root);
    // postOrder(root);

}

void signUp(int client_socket){

    char response[1024];
    char signUp_Email[50];
    char signUp_name[50];
    char signUp_nrc[50];
    char signUp_phNo[50];
    char signUp_pOrb[6];
    char signUp_curAmt[50];
    char signUp_password[50];

    snprintf(response,sizeof(response),"\n**>> SignUp Here <<**\n");
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    snprintf(response,sizeof(response),"Enter Email:");
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    memset(signUp_Email, 0, sizeof(signUp_Email));
    recv(client_socket, signUp_Email, sizeof(signUp_Email) - 1, 0);
    signUp_Email[sizeof(signUp_Email) - 1] = '\0';

    trim(signUp_Email);

    gmailValid=-1;
    gmailValidation(signUp_Email);

    if(gmailValid != -1){

        emailExist=-1;
        emailExistChecking(root,signUp_Email);

        if(emailExist==-1){

            snprintf(response,sizeof(response),"Enter Name:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            memset(signUp_name, 0, sizeof(signUp_name));
            recv(client_socket, signUp_name, sizeof(signUp_name) - 1, 0);
            signUp_name[sizeof(signUp_name) - 1] = '\0';

            trim(signUp_name);

            nrc_valid=-1;
            while(nrc_valid==-1){

                snprintf(response,sizeof(response),"Enter NRC:");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                memset(signUp_nrc, 0, sizeof(signUp_nrc));
                recv(client_socket, signUp_nrc, sizeof(signUp_nrc) - 1, 0);
                signUp_nrc[sizeof(signUp_nrc) - 1] = '\0';

                trim(signUp_nrc);
                nrcValidation(signUp_nrc);

                if(nrc_valid == -1){

                    snprintf(response,sizeof(response),"This NRC Format is unavailable!!!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);

                }
            }

            phone_valid = -1;
            while(phone_valid != 1){

                dataType_Valid = -1;
                while(dataType_Valid == -1){

                    snprintf(response,sizeof(response),"Enter Phone Number:");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);

                    memset(signUp_phNo, 0, sizeof(signUp_phNo));
                    recv(client_socket, signUp_phNo, sizeof(signUp_phNo) - 1, 0);
                    signUp_phNo[sizeof(signUp_phNo) - 1] = '\0';

                    trim(signUp_phNo);
                    int_checker(signUp_phNo);

                    if(dataType_Valid == 1){
                        db[users].phNumber = charArray_to_llu(signUp_phNo);

                    }else{
                        snprintf(response,sizeof(response),"Invalid Input!! Please enter again!!\n");
                        response[sizeof(response) - 1] = '\0';
                        send(client_socket,response,strlen(response),0);
                    }

                }

                existence=0;
                phone_validation(root,db[users].phNumber);
                if(existence==0){
                    phone_valid=1;
                }else{
                    snprintf(response,sizeof(response),"Your phone is already registered!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);
                }

            }

            snprintf(response,sizeof(response),"Enter Address:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            memset(db[users].address, 0, sizeof(db[users].address));
            recv(client_socket, db[users].address, sizeof(db[users].address) - 1, 0);
            db[users].address[sizeof(db[users].address) - 1] = '\0';

            trim(db[users].address);

            int i = 0;
            porB = -1;
            while(porB == -1){

                snprintf(response,sizeof(response),"Personal (or) Business??\nEnter '1' to apply Personal Acc/ Enter '2' to apply Business Acc:");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                memset(signUp_pOrb, 0, sizeof(signUp_pOrb));
                recv(client_socket, signUp_pOrb, sizeof(signUp_pOrb) - 1, 0);
                signUp_pOrb[sizeof(signUp_pOrb) - 1] = '\0';

                i = charArray_to_int(signUp_pOrb);
                pOrb_validation(i);

                if(porB == -1){
                    snprintf(response,sizeof(response),"Invalid Input!! Please enter again!!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);
                }

            }

            db[users].acc_level=1;
            get_limit_amount(users);
            db[users].transAmoLimitPerDay=trans_limit;
            db[users].total_loanAmount=0;

            dataType_Valid = -1;
            while(dataType_Valid == -1) {

                snprintf(response,sizeof(response),"Enter current amount:");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                memset(signUp_curAmt, 0, sizeof(signUp_curAmt));
                recv(client_socket, signUp_curAmt, sizeof(signUp_curAmt) - 1, 0);
                signUp_curAmt[sizeof(signUp_curAmt) - 1] = '\0';

                trim(signUp_curAmt);
                int_checker(signUp_curAmt);

                if(dataType_Valid == 1){
                    db[users].cur_amount = charArray_to_unsigned_fun(signUp_curAmt);
                }else{
                    snprintf(response,sizeof(response),"Invalid Input!! Please enter again!!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);
                }

            }

            strongPass = -1;
            while(strongPass == -1){

                snprintf(response,sizeof(response),"Enter Password:");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                memset(signUp_password, 0, sizeof(signUp_password));
                recv(client_socket, signUp_password, sizeof(signUp_password) - 1, 0);
                signUp_password[sizeof(signUp_password) - 1] = '\0';

                trim(signUp_password);

                existence = 0;
                passwordExistChecking(root,signUp_password);

                if(existence == 1){

                    snprintf(response,sizeof(response),"This Password registered already!\nPlease Try Again!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);
                    strongPass = -1;

                }else{

                    strongPassword(signUp_password);
                    if(strongPass == -1){

                        snprintf(response,sizeof(response),"Password format is too weak!\nPlease Try Again!\n");
                        response[sizeof(response) - 1] = '\0';
                        send(client_socket,response,strlen(response),0);

                    }
                }
            }

            db[users].id = users+100001; //generate User ID
            copy_two_char_array(db[users].email,signUp_Email);
            copy_two_char_array(db[users].name,signUp_name);
            copy_two_char_array(db[users].nrc,signUp_nrc);
            copy_two_char_array(db[users].password,signUp_password);

            root= insertNode(root,&db[users]);
            recordData_to_file(root);

            const char *string1 = "Your acc has been successfully created...";
            const char *string2 = "This is your ID: ";
            const char *string3 = "Please use it when you LogIn:";
            const char *string4 = "**>> Thanks For Your Registration <<**";

            char message[4096];
            snprintf(message, sizeof(message), "\n%s\n\n%s%u\n%s\n\n%s\n", string1,
                     string2, db[users].id, string3, string4);

            int bytes_sent = send(client_socket, message, strlen(message), 0);
            if (bytes_sent == -1) {
                fprintf(stderr, "send() failed. (%d)\n", GETSOCKETERRNO());
                CLOSESOCKET(client_socket);
            }

            exit(0);

        }else{

            snprintf(response,sizeof(response),"Your gmail already taken!\n");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);
            signUp(client_socket);

        }

    }else{

        snprintf(response,sizeof(response),"Invalid gmail format!\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);
        signUp(client_socket);

    }

}

void logIN(int client_socket){

    char response[1024];
    char login_id[50];
    char login_Password[50];
    unsigned int login_ID=0;

    userIndex=-1;
    two_charArray=-1;

    while( userIndex==-1 || two_charArray==-1 ){

        snprintf(response,sizeof(response),"\n**>> LogIn Here <<**\n");// send response
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        snprintf(response,sizeof(response),"Enter Your ID:");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        memset(login_id, 0, sizeof(login_id));
        recv(client_socket, login_id, sizeof(login_id) - 1, 0); // receive client's response
        login_id[sizeof(login_id) - 1] = '\0';

        trim(login_id); // to remove any trailing spaces or newline characters

        dataType_Valid = -1;
        int_checker(login_id);  // to detect wrong user input!

        if(dataType_Valid == 1){ //No invalid input!

            login_ID = charArray_to_unsigned_fun(login_id);

            snprintf(response,sizeof(response),"Enter Your Password:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            memset(login_Password, 0, sizeof(login_Password));
            recv(client_socket, login_Password, sizeof(login_Password) - 1, 0);
            login_Password[sizeof(login_Password) - 1] = '\0';

            trim(login_Password);

            searching(root,login_ID);

            compare_two_charArray(db[userIndex].password,login_Password);

        }else{

            snprintf(response,sizeof(response),"Invalid Input!! Please enter again!!\n");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

        }

        if( userIndex==-1 || two_charArray==-1 ){

            snprintf(response,sizeof(response),"Your login credential is wrong!\n");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

        }
    }

    snprintf(response,sizeof(response),"Welcome Mr/Mrs: %s\n",db[userIndex].name);
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    snprintf(response,sizeof(response),"Your current amount : %u\n",db[userIndex].cur_amount);
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    user_sector(client_socket, userIndex);

}

void user_sector(int client_socket, int index){

    char response[1024];
    char user_Opt[6];

    snprintf(response,sizeof(response),"\n\n**>> User Sector <**\n");
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    char message[4096];
    snprintf(message, sizeof(message), "\nAcc Balance: %u\n\nName: %s\nEmail: %s\nPhone: %llu\nNRC: %s\nAddress: %s\nPassword: %s\nAcc Type: %s\nAcc Level: %d\n\n",
             db[index].cur_amount,db[index].name,db[index].email,db[index].phNumber,db[index].nrc,db[index].address,db[index].password,db[index].pOrb,db[index].acc_level);
    message[sizeof(message) - 1] = '\0';
    send(client_socket, message, strlen(message), 0);

    snprintf(response,sizeof(response),"Press 1 to Transfer Money:\nPress 2 to Cash Out:\nPress 3 to Cash In\nPress 4 To Loan:\n"
                                       "Press 5 to Update Your Information:\nPress 6 History:\nPress 7 Exit:\n");
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    recv(client_socket, user_Opt, sizeof(user_Opt) - 1, 0);
    user_Opt[sizeof(user_Opt) - 1] = '\0';

    user_option = charArray_to_int(user_Opt);

    if (user_option == 1) {

        snprintf(response,sizeof(response),"\n*> Transfer <*\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);
        transfer(userIndex, client_socket);

    } else if (user_option == 2) {

        snprintf(response,sizeof(response),"\n*> Cash Out <*\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);
        with_draw(userIndex, client_socket);

    } else if (user_option == 3){

        snprintf(response,sizeof(response),"\n*> Cash In <*\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);
        cashIn(userIndex, client_socket);

    } else if (user_option == 4){

        snprintf(response,sizeof(response),"\n*> Loan <*\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);
        loan(userIndex, client_socket);

    } else if (user_option == 5){

        snprintf(response,sizeof(response),"\n*> Account Information <*\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);
        update_information(userIndex, client_socket);

    } else if (user_option == 6){

        snprintf(response,sizeof(response),"\n*> History <*\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);
        history(userIndex, client_socket);

    } else if (user_option == 7){

        snprintf(response,sizeof(response),"***>>> Thanks For Being Our Customer <<<***\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        recordData_to_file(root);
        exit(0);

    }else{

        snprintf(response,sizeof(response),"\nInvalid Input!!!\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);
        welcome(client_socket);

    }

}

void trim(char *str) {
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

void emailExistChecking(struct Node *root,char toCheck[50]){

    two_charArray=-1;
    if(root != NULL){

        compare_two_charArray(toCheck,root->seed->email);

        if(two_charArray==1){
            emailExist=1;
            emailExistChecking(NULL,toCheck);
        }
        emailExistChecking(root->left,toCheck);
        emailExistChecking(root->right,toCheck);

    }
}

void gmailValidation(char mailToValidate[50]){

    int endPoint = charCounting(mailToValidate);
    char form[10]={'@','g','m','a','i','l','.','c','o','m'};
    int count=0;

    for(int i=0; i<endPoint; i++){
        if(mailToValidate[i] == '@' || mailToValidate[i] ==' '){
            break;
        }else{
            count++;
        }
    }

    int toCheck=0;
    for(int i=0 ; i<endPoint ; i++){
        if( mailToValidate[count] != form[i]){
            break;
        } else{
            count++;
            toCheck++;
        }
    }

    if( toCheck == 10){
        gmailValid=1;
    }
}

void nrcValidation(char nrc_toCheck[50]){ //Only for the Myanmar NRC type

    int digitCount=0;
    int adjust=0;
    char temp[6];
    char format[6]={'/','(',')'};

    int nrc_counter =charCounting(nrc_toCheck); //to get times for looping
    for(int i=0; i< nrc_counter ; i++){

        if( nrc_toCheck[i] == '/' || nrc_toCheck[i] == '(' || nrc_toCheck[i] == ')'){
            temp[adjust]=nrc_toCheck[i];
            adjust++;

            if(adjust>3){ //make sure to prepare for wrong user input
                break;
            }
        }

        if(adjust==3 && nrc_toCheck[i]>47 && nrc_toCheck[i]<58){ //not to include other chars in numbers
            digitCount++;
        }

        if(digitCount==6 && nrc_toCheck[i+1]!='\0'){
            digitCount=0;
            break;
        }
    }

    if(digitCount == 6){ //approve standard characters in NRC format after NRC number check
        nrc_valid=1;
    }
}

void int_checker(char input[]){

    int counter = charCounting(input);
    int same_counter = 0;

    for(int x=0; x<counter; x++){
        if(input[x]<48 || input[x]>57){
            dataType_Valid = -1;
            break;
        }
        same_counter++;
    }

    if(counter == same_counter){
        dataType_Valid = 1;
    }

}

void phone_validation(struct Node *root,unsigned long long int phone){

    if(root!=NULL){
        if(phone==root->seed->phNumber){
            existence=root->seed->index;
            phone_validation(NULL,phone);
        }
        phone_validation(root->left,phone);
        phone_validation(root->right,phone);
    }

}

void pOrb_validation(int i){

    char p_format[20]={'p','e','r','s','o','n','a','l'};
    char b_format[20]={'b','u','s','i','n','e','s','s'};

    if(i==1){
        copy_two_char_array(db[users].pOrb,p_format);
        porB=i;
    }else if(i==2){
        copy_two_char_array(db[users].pOrb,b_format);
        porB=i;
    }else{
        porB=-1;
        printf("Invalid Input!!\n");
    }
}

void passwordExistChecking(struct Node *root,char pass[50]){
    two_charArray=-1;
    if(root!=NULL){

        compare_two_charArray(root->seed->password,pass);
        if(two_charArray==1){
            existence=1;
            passwordExistChecking(NULL,pass);
        }
        passwordExistChecking(root->left,pass);
        passwordExistChecking(root->right,pass);

    }
}

void strongPassword(char pass[50]){

    int i=0;
    int special=0;
    int numberChar=0;
    int capChar=0;
    int smallChar=0;
    int pass_counter = charCounting(pass);

    while (strongPass != 1){

        if( pass[i] >= 33 && pass[i] <= 42){
            special++;
        }else if( pass[i] >= 48 && pass[i] <= 57){
            numberChar++;
        }else if( pass[i] >= 65 && pass[i] <= 90){
            capChar++;
        }else if( pass[i] >= 97 && pass[i] <= 122) {
            smallChar++;
        }
        i++;

        if( special > 0 && numberChar >0 && capChar > 0 && smallChar >0 ){
            strongPass=1;
            break;
        }
        if( i == pass_counter){
            strongPass=-1;
            break;
        }
    }
}

void searching(struct Node *root,unsigned int data){

    if(root!=NULL){

        if(root->seed->id == data){
            userIndex=root->seed->index;

        }else if(root->seed->id > data){
            if(data == root->seed->id){
                userIndex=root->seed->index;
            }
            searching(root->left,data);

        }else if(root->seed->id < data){
            if(data == root->seed->id){
                userIndex=root->seed->index;
            }
            searching(root->right,data);

        }
    }else{
        userIndex=-1;
        printf("Data Not Found!!");
    }
}

void compare_two_charArray(char first[100] , char second[100]){

    int first_counter =  charCounting(first);
    int second_counter = charCounting(second);
    int same_counter = 0;

    if( first_counter == second_counter) {
        for (register int i = 0; i < first_counter; i++) {
            if(first[i] != second[i]){
                two_charArray = -1;
                break;
            }
            same_counter++;
        }

        if( first_counter == same_counter){
            two_charArray = 1;
        }
    }

}

void copy_two_char_array(char receiver[200],char transmitter[200]){

    int receiver_counter = charCounting(receiver);
    for(int i=0; i<receiver_counter; i++){
        receiver[i] = '\0';
    }

    int transmit_counter = charCounting(transmitter);
    for(int i=0; i<transmit_counter; i++){
        receiver[i] = transmitter[i];
    }

}

int charCounting(char toCount[50]){
    int charCount = 0;

    for(int gcc=0; gcc<50 ; gcc++){
        if( toCount[gcc] == '\0'){
            break;
        }
        charCount++;
    }
    return charCount;
}

void transfer(int index, int client_socket){

    char response[1024];
    unsigned long long int phone;
    unsigned int amount_to_transfer=0;

    existence = -1;
    while(existence == -1){

        snprintf(response,sizeof(response),"Enter receive phone number:");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        memset(str_input, 0, sizeof(str_input));
        recv(client_socket, str_input, sizeof(str_input) - 1, 0);
        str_input[sizeof(str_input) - 1] = '\0';

        int_checker(str_input);

        if(dataType_Valid == 1){

            phone = charArray_to_llu(str_input);
            phone_validation(root,phone);

            if(existence == -1){
                snprintf(response,sizeof(response),"Phone number does not belong to any user!!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

            }
            if(db[index].phNumber == db[existence].phNumber){
                snprintf(response,sizeof(response),"Invalid Option!!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);
                existence=-1;

            }
        }else{
            snprintf(response,sizeof(response),"Invalid Input!!\n");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);
            existence = -1;

        }
    }

    phone_found = existence;

    snprintf(response,sizeof(response),"Are you sure to send for %s email: %s:\n",
             db[phone_found].name, db[phone_found].email);
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    dataType_Valid=-1;
    while (dataType_Valid==-1){

        snprintf(response,sizeof(response),"Enter your amount: to transfer:");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        memset(str_input, 0, sizeof(str_input));
        recv(client_socket, str_input, sizeof(str_input) - 1, 0);
        str_input[sizeof(str_input) - 1] = '\0';

        trim(str_input);
        int_checker(str_input);
        amount_to_transfer = charArray_to_unsigned_fun(str_input);

        if(dataType_Valid==1){
            if(db[index].cur_amount - 1000 > amount_to_transfer){
                break;

            }else{

                snprintf(response,sizeof(response),"Not sufficient balance:\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);
                dataType_Valid=-1;

            }
        }
    }

    two_charArray = -1;
    char tran_pass[50];

    while(two_charArray == -1){

        snprintf(response,sizeof(response),"Enter password to proceed transaction:");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        memset(tran_pass, 0, sizeof(tran_pass));
        recv(client_socket, tran_pass, sizeof(tran_pass) - 1, 0);
        tran_pass[sizeof(tran_pass) - 1] = '\0';

        trim(tran_pass);
        compare_two_charArray(db[index].password, tran_pass);

        if(two_charArray == -1){

            snprintf(response,sizeof(response),"Wrong credential:\n");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

        }

    }

    calculation_transAmt_on_sameDay(amount_to_transfer, client_socket);

    if(transaction_valid != -1){
        transfer_money(index, phone_found, amount_to_transfer, client_socket);
        printf("\n[ Clients Data ]\n");
        printing_data();
    }

    user_sector(client_socket, index);

}

void calculation_transAmt_on_sameDay(unsigned int amount, int client_socket){

    char response[1024];
    get_current_day();
    transaction_valid=0;

    if(currentDay != db[userIndex].tracking.trans_perDay){
        db[userIndex].tracking.totalTransfer=0;
        db[phone_found].tracking.totalReceive=0;
    }

    total_currentAmt_transfer=db[userIndex].tracking.totalTransfer + amount;
    total_currentAmt_receive=db[phone_found].tracking.totalReceive + amount;

    if(total_currentAmt_transfer > db[userIndex].transAmoLimitPerDay){

        transaction_valid=-1;
        snprintf(response,sizeof(response),"\nYour Transaction Limit Over!!\n"
                                           "You can transfer %u per day!!\nYou transferred %u today!!..\n"
                                           , db[userIndex].transAmoLimitPerDay, db[userIndex].tracking.totalTransfer);
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

    }

    if(total_currentAmt_receive > db[phone_found].transAmoLimitPerDay){

        transaction_valid=-1;
        snprintf(response,sizeof(response),"\n%s Transaction Limit Over!!\nCannot Receive Your Transfer!!..\n"
                 , db[phone_found].name);
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

    }

    if(total_currentAmt_transfer <= db[userIndex].transAmoLimitPerDay){

        db[userIndex].tracking.totalTransfer = total_currentAmt_transfer;

        if(total_currentAmt_receive <= db[phone_found].transAmoLimitPerDay){

            db[phone_found].tracking.totalReceive = total_currentAmt_receive;

        }

    }

}

void transfer_money(int transfer , int receiver , unsigned int amount, int client_socket){

    char response[1024];

    snprintf(response,sizeof(response),"loading to transfer.....\n");
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    db[transfer].cur_amount = db[transfer].cur_amount-amount;
    db[receiver].cur_amount = db[receiver].cur_amount+amount;

    snprintf(response,sizeof(response),"Transaction complete!\n");
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    transaction_record(transfer,receiver,'t',amount);
    transaction_record(transfer,receiver,'r',amount);

}

void transaction_record(int transfer , int receiver, char who, unsigned int amount){
// Transaction:Transfer[-]TransAmount-to-Receiver-time

    int trans_name_counter = charCounting(db[transfer].name);
    int receive_name_counter = charCounting(db[receiver].name);

    integer_to_char(amount);
    int amount_counter = charCounting(int_to_char_array_data);

    char transfer_label[20]={'T','r','a','n','s','a','c','t','i','o',
                             'n',':','T','r','a','n','s','f','e','r'};
    char to[4]={'-','t','o','-'};
    char transfer_sign[3]={'[','-',']'};

    if(who == 't'){

        int index_point=0;
        for(int i=0; i<20; i++){ //Transaction:Transfer
            db[transfer].record[db[transfer].tracking.recordCounter].note[i] = transfer_label[i];
            index_point++;
        }

        for(int a=0; a<3; a++){ //[-]
            db[transfer].record[db[transfer].tracking.recordCounter].note[index_point] = transfer_sign[a];
            index_point++;
        }

        for(int amt=0; amt<amount_counter; amt++){ //Transferred Amount
            db[transfer].record[db[transfer].tracking.recordCounter].note[index_point] = int_to_char_array_data[amt];
            index_point++;
        }

        for(int a=0; a<4; a++){ //-to-
            db[transfer].record[db[transfer].tracking.recordCounter].note[index_point] = to[a];
            index_point++;
        }

        for (int n=0; n<receive_name_counter; ++n ){ //Receiver Name
            db[transfer].record[db[transfer].tracking.recordCounter].note[index_point] = db[receiver].name[n];
            index_point++;
        }

        get_time();
        for(int i=0; i<25; i++){
            db[transfer].record[db[transfer].tracking.recordCounter].note[index_point] = Ctime[0].c_time[i];
            index_point++;
        }

        db[transfer].tracking.trans_perDay = currentDay; //to calculate amount on the same day
        db[transfer].tracking.recordCounter++; //to adjust struct record[index]


    }else{
        // Transaction:Receive[+]TransAmount-from-WhoTransfer-time

        char receive_label[19]={'T','r','a','n','s','a','c','t','i','o','n',':','R','e','c','e','i','v','e'};
        char from[6]={'-','f','r','o','m','-'};
        char receive_sign[3]={'[','+',']'};

        int index_point=0;

        for(int a=0; a<19; a++){ // Transaction:Receive
            db[receiver].record[db[receiver].tracking.recordCounter].note[index_point] = receive_label[a];
            index_point++;
        }

        for(int a=0; a<3; a++){ // [+]
            db[receiver].record[db[receiver].tracking.recordCounter].note[index_point] = receive_sign[a];
            index_point++;
        }

        for(int amt=0; amt<amount_counter; amt++){
            db[receiver].record[db[receiver].tracking.recordCounter].note[index_point] = int_to_char_array_data[amt];
            index_point++;
        }

        for(int a=0; a<6; a++){ // -from-
            db[receiver].record[db[receiver].tracking.recordCounter].note[index_point] = from[a];
            index_point++;
        }

        for(int a=0; a<trans_name_counter; a++){ //WhoTransfer
            db[receiver].record[db[receiver].tracking.recordCounter].note[index_point] = db[transfer].name[a];
            index_point++;
        }

        get_time();
        for(int a=0; a<25; a++){
            db[receiver].record[db[receiver].tracking.recordCounter].note[index_point] = Ctime[0].c_time[a];
            index_point++;
        }


        db[receiver].tracking.trans_perDay = currentDay;//to calculate amount on the same day
        db[receiver].tracking.recordCounter++;  //to adjust struct record[index]

    }
}

void calculate_withdrawAmt_sameDay(int index, unsigned int amount, int client_socket){

    char response[1024];
    get_current_day();

    if(currentDay!=db[index].tracking.cashOut_perDay){
        db[index].tracking.totalCashOut=0;
    }

    total_CashOut = db[index].tracking.totalCashOut+amount;

    if(total_CashOut > db[index].transAmoLimitPerDay){

        cashOut_limit=-1;
        snprintf(response,sizeof(response),"\nCashOut Limit Over For Today!!\nYou can withdraw %u per day!!\n"
                 , db[index].transAmoLimitPerDay);
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

    }else{
        cashOut_limit=0;
        db[index].tracking.totalCashOut = total_CashOut;
    }

    snprintf(response,sizeof(response),"CashOut Amount for Today:%u\n", db[index].tracking.totalCashOut);
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

}

void with_draw(int index, int client_socket){

    char response[1024];
    unsigned int withdrawAmt=0;
    char withdraw_pass[50];

    snprintf(response,sizeof(response),"Name :%s\nCurrent Amount :%u\n", db[index].name, db[index].cur_amount);
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    snprintf(response,sizeof(response),"Enter your withdraw amount:");
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    memset(str_input, 0, sizeof(str_input));
    recv(client_socket, str_input, sizeof(str_input) - 1, 0);
    str_input[sizeof(str_input) - 1] = '\0';

    trim(str_input);
    int_checker(str_input);

    if(dataType_Valid==-1){
        with_draw(index, client_socket);
    }

    withdrawAmt = charArray_to_unsigned_fun(str_input);

    if(withdrawAmt >= db[index].cur_amount-1000){

        snprintf(response,sizeof(response),"\nInsufficient Amount: %u\n\n",withdrawAmt-(db[index].cur_amount-1000));
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        with_draw(index, client_socket);

    }else{

        snprintf(response,sizeof(response),"Enter your password to proceed withdraw:");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        memset(withdraw_pass, 0, sizeof(withdraw_pass));
        recv(client_socket, withdraw_pass, sizeof(withdraw_pass) - 1, 0);
        withdraw_pass[sizeof(withdraw_pass) - 1] = '\0';

        trim(withdraw_pass);

        two_charArray=-1;
        compare_two_charArray(db[index].password,withdraw_pass);

        if(two_charArray==-1){

            snprintf(response,sizeof(response),"\nWrong credential:\n\n");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            with_draw(index,client_socket);

        }else{

            calculate_withdrawAmt_sameDay(index,withdrawAmt,client_socket);

            if(cashOut_limit==-1){

                user_sector(client_socket, index);

            }else{

                int withdraw_option=0;

                db[index].cur_amount = db[index].cur_amount-withdrawAmt;

                snprintf(response,sizeof(response),"\nCash Out Successfully Proceeded:\nYour current amount: %u\n"
                         ,db[index].cur_amount);
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                withdraw_record(index,db[index].cur_amount,withdrawAmt);
                printf("\n[ Clients Data ]\n");
                printing_data();

                snprintf(response,sizeof(response),"Press 1 Withdraw Again:\nPress 2 Back to User Sector:\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                memset(str_input, 0, sizeof(str_input));
                recv(client_socket, str_input, sizeof(str_input) - 1, 0);
                str_input[sizeof(str_input) - 1] = '\0';

                withdraw_option = charArray_to_int(str_input);

                if(withdraw_option==1){
                    with_draw(index, client_socket);

                }else if(withdraw_option==2){
                    user_sector(client_socket, index);

                }else{
                    snprintf(response,sizeof(response),"Invalid Option:\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);
                    user_sector(client_socket, index);

                }
            }
        }
    }
}

void withdraw_record(int index,unsigned int cur_amt,unsigned int withdrawAmt){
//Cash_Out:$1000-time-Remain_Balance:$29000

    char cash_out_display[10]={'C','a','s','h','_','O','u','t',':','$'};

    char balance_display[17]={'-','R','e','m','a','i','n','_',
                              'B','a','l','a','n','c','e',':','$'};

    if(withdrawAmt != 0){

        integer_to_char(withdrawAmt);
        int witdrwl_amt_counter = charCounting(int_to_char_array_data);

        int index_point=0;

        for(int i=0; i<10; i++){ //Cash_Out:$
            db[index].record[db[index].tracking.recordCounter].note[i] = cash_out_display[i];
            index_point++;
        }

        for(int i=0; i<witdrwl_amt_counter; i++){ //Amount
            db[index].record[db[index].tracking.recordCounter].note[index_point] = int_to_char_array_data[i];
            index_point++;
        }

        get_time();
        for(int a=0; a<25; a++){ //time
            db[index].record[db[index].tracking.recordCounter].note[index_point] = Ctime[0].c_time[a];
            index_point++;
        }

        for(int i=0; i<17; i++){ //-Remain_Balance:$
            db[index].record[db[index].tracking.recordCounter].note[index_point] = balance_display[i];
            index_point++;
        }

        integer_to_char(cur_amt);
        int cur_amt_counter = charCounting(int_to_char_array_data);

        for(int i=0; i<cur_amt_counter; i++){ //Current Amount
            db[index].record[db[index].tracking.recordCounter].note[index_point] = int_to_char_array_data[i];
            index_point++;
        }

        db[index].tracking.cashOut_perDay = currentDay;
        db[index].tracking.recordCounter++;

    }
}

void cashIn(int index, int client_socket){

    char response[1024];
    unsigned int cashIn_Amt=0;
    char cashIn_pass[50];
    int cashIn_option=0;

    snprintf(response,sizeof(response),"Name :%s\nCurrent Amount :%u\nEnter your Cash_In amount:"
             ,db[index].name,db[index].cur_amount);
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    memset(str_input, 0, sizeof(str_input));
    recv(client_socket, str_input, sizeof(str_input) - 1, 0);
    str_input[sizeof(str_input) - 1] = '\0';

    trim(str_input);
    int_checker(str_input);
    cashIn_Amt = charArray_to_unsigned_fun(str_input);

    if(dataType_Valid==-1){

        cashIn(index, client_socket);

    }else{

        snprintf(response,sizeof(response),"Enter your password to proceed withdraw:");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        memset(cashIn_pass, 0, sizeof(cashIn_pass));
        recv(client_socket, cashIn_pass, sizeof(cashIn_pass) - 1, 0);
        cashIn_pass[sizeof(cashIn_pass) - 1] = '\0';

        trim(cashIn_pass);

        two_charArray=-1;
        compare_two_charArray(db[index].password,cashIn_pass);

        if(two_charArray==-1){

            snprintf(response,sizeof(response),"\nWrong credential:\n\n");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            cashIn(index,client_socket);

        }else{

            db[index].cur_amount = db[index].cur_amount+cashIn_Amt;

            snprintf(response,sizeof(response),"\nCash_In Proceeded:\nYour current amount: %u\n",db[index].cur_amount);
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            cashIn_record(index,db[index].cur_amount,cashIn_Amt);
            printf("\n[ Clients Data ]\n");
            printing_data();

            snprintf(response,sizeof(response),"Press 1 to Cash_In more:\nPress 2 Back to User Sector:\n");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            memset(str_input, 0, sizeof(str_input));
            recv(client_socket, str_input, sizeof(str_input) - 1, 0);
            str_input[sizeof(str_input) - 1] = '\0';

            trim(str_input);
            cashIn_option = charArray_to_int(str_input);

            if(cashIn_option==1){
                cashIn(index,client_socket);

            }else if(cashIn_option==2){
                user_sector(client_socket, index);

            }else{
                snprintf(response,sizeof(response),"Invalid Option:\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);
                user_sector(client_socket, index);

            }
        }
    }
}

void cashIn_record(int index, unsigned int cur_amt, unsigned int cashInAmt){
//Cash_In:$9000-time-Current_Balance:$109000

    char cash_in_display[9]={'C','a','s','h','_','i','n',':','$'};

    char balance_display[18]={'-','C','u','r','r','e','n','t','_',
                              'B','a','l','a','n','c','e',':','$'};

    if(cashInAmt != 0){

        integer_to_char(cashInAmt);
        int cashIn_amt_counter = charCounting(int_to_char_array_data);

        int index_point=0;

        for(int i=0; i<9; i++){ //Cash_In:$
            db[index].record[db[index].tracking.recordCounter].note[i] = cash_in_display[i];
            index_point++;
        }

        for(int i=0; i<cashIn_amt_counter; i++){ //Amount
            db[index].record[db[index].tracking.recordCounter].note[index_point] = int_to_char_array_data[i];
            index_point++;
        }

        get_time();
        for(int a=0; a<25; a++){ //time
            db[index].record[db[index].tracking.recordCounter].note[index_point] = Ctime[0].c_time[a];
            index_point++;
        }

        for(int i=0; i<18; i++){ //-Current_Balance:$
            db[index].record[db[index].tracking.recordCounter].note[index_point] = balance_display[i];
            index_point++;
        }

        integer_to_char(cur_amt);
        int cur_amt_counter = charCounting(int_to_char_array_data);

        for(int i=0; i<cur_amt_counter; i++){ //Current Amount
            db[index].record[db[index].tracking.recordCounter].note[index_point] = int_to_char_array_data[i];
            index_point++;
        }

        db[index].tracking.recordCounter++;

    }
}

void loan(int index, int client_socket){

    char response[1024];
    double new_loanAmount;
    int agree=0;

    const char *string1 = "Testing Purpose >> For the testing purpose, interest rate calculation is programmed with minutes. "
                          "For instance, 1 Minute increment is equal to 1 Month so that 12 Minute will be 1 year.";

    const char *string2 = ">> Terms & Condition <<";

    const char *string3 = "Limitation of the loan is depend on the amount of transition limit per day. Once you loan, the repayment must be "
                          "settled within 1 year and loan process will be unavailable. INTEREST RATE & DURATION is FIXED. If your repayment is "
                          "delayed over the due date, loan_service will be unavailable while INTEREST RATE still increase. After due date, you are "
                          "welcome for negotiation, you can share your difficulties and deal with us without worries and we will be looking forward to yours. "
                          "However,  if there were no responsible actions for your payment, bank will follow the action legally in terms of "
                          "legitimate regulations.";


    snprintf(response,sizeof(response),"\n%s\n\n%s\n\n%s\n", string1, string2, string3);
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    snprintf(response,sizeof(response),"\nPress 1: if you agree/ Press 2: Back to User Sector\n");
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    memset(str_input, 0, sizeof(str_input));
    recv(client_socket, str_input, sizeof(str_input) - 1, 0);
    str_input[sizeof(str_input) - 1] = '\0';

    agree = charArray_to_int(str_input);

    if(agree == 1){

        snprintf(response,sizeof(response),"\nAnnual Interest Rate(fixed): %.2f%%\nMonthly Interest Rate(fixed): %.2f%%\n",
                 annual_InterestRate, monthly_InterestRate);
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        if(db[index].tracking.total_loan_payment != 0){

            current_payment(index);

            snprintf(response,sizeof(response),"Total Repayment for each month: $%.2f\nTotal Repayment at the Due Date: $%.2f\n",
                     db[index].tracking.loan_payment, db[index].tracking.total_loan_payment);
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            if(total_interestRate_Minute <= 12){

                snprintf(response,sizeof(response),"%d month left from now\n", 12-total_interestRate_Minute);
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

            }else{

                snprintf(response,sizeof(response),"%d month Over Due for the repayment!!Loan Service Unavailable!!\n",
                         total_interestRate_Minute-12);
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                int repaid_opt=0;
                dataType_Valid=-1;
                while(dataType_Valid == -1){

                    snprintf(response,sizeof(response),"\nPress 1: Repay for Loan:/ Press 2: Back to User Sector:");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);

                    memset(str_input, 0, sizeof(str_input));
                    recv(client_socket, str_input, sizeof(str_input) - 1, 0);
                    str_input[sizeof(str_input) - 1] = '\0';

                    trim(str_input);
                    int_checker(str_input);

                    repaid_opt = charArray_to_int(str_input);

                    if(repaid_opt == 1){

                        dataType_Valid=1;
                        repayment(index, client_socket);

                    }else if(repaid_opt == 2){

                        dataType_Valid=1;
                        user_sector(client_socket, index);

                    }else{

                        snprintf(response,sizeof(response),"Invalid Input!!\n");
                        response[sizeof(response) - 1] = '\0';
                        send(client_socket,response,strlen(response),0);

                        dataType_Valid=-1;

                    }
                }
            }
        }

        loanOrRepay = -1;
        while(loanOrRepay == -1){

            snprintf(response,sizeof(response),"\nPress 1: Loan/ Press 2: Repay for Loan:/ Press 3: Back to User Sector:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            memset(str_input, 0, sizeof(str_input));
            recv(client_socket, str_input, sizeof(str_input) - 1, 0);
            str_input[sizeof(str_input) - 1] = '\0';

            trim(str_input);
            int_checker(str_input);

            loanOrRepay = charArray_to_int(str_input);

            if(dataType_Valid == -1){

                loanOrRepay = -1;

            }else{

                if(loanOrRepay == 1){

                    snprintf(response,sizeof(response),"\nYour Current Balance: %u",db[index].cur_amount);
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);

                    snprintf(response,sizeof(response),"\nEnter the loan amount:");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);

                    memset(str_input, 0, sizeof(str_input));
                    recv(client_socket, str_input, sizeof(str_input) - 1, 0);
                    str_input[sizeof(str_input) - 1] = '\0';

                    new_loanAmount = strtod(str_input,NULL); // to avoid slight difference when changing str_input to double dataType

                    proceed_loan(index,new_loanAmount,client_socket);

                }else if(loanOrRepay == 2){

                    repayment(index, client_socket);

                }else if(loanOrRepay == 3){

                    user_sector(client_socket, index);

                }else{

                    loanOrRepay = -1;

                }
            }
        }

        int loan_option = 0;

        snprintf(response,sizeof(response),"\nPress 1 Back to User Sector:\nPress 2 to Exit:\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        memset(str_input, 0, sizeof(str_input));
        recv(client_socket, str_input, sizeof(str_input) - 1, 0);
        str_input[sizeof(str_input) - 1] = '\0';

        loan_option = charArray_to_int(str_input);

        if(loan_option == 1){
            user_sector(client_socket, index);

        }else if(loan_option == 2){

            snprintf(response,sizeof(response),"***>>> Thanks For Being Our Customer <<<***");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            recordData_to_file(root);
            exit(0);

        }else{

            snprintf(response,sizeof(response),"Invalid Option:\n");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            user_sector(client_socket, index);

        }

    }else if(agree == 2){

        user_sector(client_socket, index);

    }else{

        snprintf(response,sizeof(response),"Invalid Input!!");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        user_sector(client_socket, index);

    }

}

void current_interest_rate_time(int index){

    int loanTime=0;
    char cur_loan_time[2];

    get_time();

    cur_loan_time[0] = Ctime[0].c_time[9];
    cur_loan_time[1] = Ctime[0].c_time[10];
    loanTime = charArray_to_int(cur_loan_time);
    cur_interest_rateMinute = loanTime * 24 * 60;
    //printf("day: %d ",loanTime);

    cur_loan_time[0] = Ctime[0].c_time[12];
    cur_loan_time[1] = Ctime[0].c_time[13];
    loanTime = charArray_to_int(cur_loan_time);
    cur_interest_rateMinute += loanTime * 60;
    //printf("hour: %d ",loanTime);

    cur_loan_time[0] = Ctime[0].c_time[15];
    cur_loan_time[1] = Ctime[0].c_time[16];
    loanTime = charArray_to_int(cur_loan_time);
    cur_interest_rateMinute += loanTime;
    //printf("min: %d\n",loanTime);

    //printf("Current No. of Min for Interest: %d\n",cur_interest_rateMinute);
}

void started_interest_rate_time(int index){

    int started_loanTime=0;
    char started_loan_time[2];

    if(db[index].tracking.loanTime_rec_counter > 0) {

        started_loan_time[0] = db[index].tracking.loanTime_Rec[9];
        started_loan_time[1] = db[index].tracking.loanTime_Rec[10];
        started_loanTime = charArray_to_int(started_loan_time);
        started_interest_rateMinute = started_loanTime * 24 * 60;
        //printf("day: %d ", started_loanTime);

        started_loan_time[0] = db[index].tracking.loanTime_Rec[12];
        started_loan_time[1] = db[index].tracking.loanTime_Rec[13];
        started_loanTime = charArray_to_int(started_loan_time);
        started_interest_rateMinute += started_loanTime * 60;
        //printf("hour: %d ", started_loanTime);

        started_loan_time[0] = db[index].tracking.loanTime_Rec[15];
        started_loan_time[1] = db[index].tracking.loanTime_Rec[16];
        started_loanTime = charArray_to_int(started_loan_time);
        started_interest_rateMinute += started_loanTime;
        //printf("min: %d\n", started_loanTime);

    }else{
        started_interest_rateMinute = 0;
    }
    //printf("Started No. of Min for Interest: %d\n",started_interest_rateMinute);
}

void current_payment(int index){

    double cur_interestRate;
    double repaid_interestRate;
    double cur_interest_amount;
    int month_left=0;

    current_interest_rate_time(index);
    started_interest_rate_time(index);

    if(started_interest_rateMinute != 0) {

        total_interestRate_Minute = cur_interest_rateMinute - started_interest_rateMinute;
        month_left = 12 - total_interestRate_Minute;

        if (db[index].tracking.repaid_switch > 0) {

            if(month_left > 0){
                db[index].tracking.loan_payment = db[index].tracking.total_loan_payment  / month_left;

            }else if(month_left < 0){
                cur_interestRate = (total_interestRate_Minute - 12) * monthly_InterestRate;
                cur_interest_amount = (db[index].total_loanAmount * cur_interestRate)/100;
                db[index].tracking.loan_payment = db[index].tracking.total_loan_payment + cur_interest_amount;

            }else if(month_left == 0){
                db[index].tracking.loan_payment = db[index].tracking.total_loan_payment;
            }

        } else {

            if(month_left > 0) {
                db[index].tracking.loan_payment = db[index].tracking.total_loan_payment / month_left;

            }else if(month_left < 0){
                cur_interestRate = total_interestRate_Minute - 12 * monthly_InterestRate;
                cur_interest_amount = (db[index].total_loanAmount * cur_interestRate)/100;
                db[index].tracking.loan_payment = db[index].tracking.total_loan_payment + cur_interest_amount;

            }else if(month_left == 0){
                db[index].tracking.loan_payment = db[index].tracking.total_loan_payment;
            }

        }

    }

}

void proceed_loan(int index, double loanAmt, int client_socket){

    char response[1024];
    double interestAmount;
    double total_interestAmount;
    double total_dueDate_payment;

    if(db[index].total_loanAmount == 0) {

        interestAmount = (loanAmt * monthly_InterestRate) / 100;
        total_interestAmount = (loanAmt * annual_InterestRate) / 100;

        db[index].total_loanAmount = loanAmt;
        db[index].tracking.total_loan_payment = loanAmt + total_interestAmount;
        db[index].tracking.loan_payment = db[index].total_loanAmount  / 12;

        snprintf(response,sizeof(response),"\nLoan Amount: $%.2f\nMonthly Payment: $%.2f\n"
                                           "Total Interest Amount: $%.2f\nTotal Repayment: $%.2f\n",
                 loanAmt, db[index].tracking.loan_payment,
                 total_interestAmount, db[index].tracking.total_loan_payment);

        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        int loanOption = -1;
        while(loanOption == -1){  // looping work 1 time itself when write the condition equal to 0?

            snprintf(response,sizeof(response),"Press 1: to proceed loan/ Press 2: back to User Sector:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            memset(str_input, 0, sizeof(str_input)); //make sure str_input to initiate with zero
            recv(client_socket, str_input, sizeof(str_input) - 1, 0);
            str_input[sizeof(str_input) - 1] = '\0';

            loanOption = charArray_to_int(str_input);

            if(loanOption == 1){
                two_charArray = -1;

            }else if(loanOption == 2){
                user_sector(client_socket, index);

            }else{

                snprintf(response,sizeof(response),"Invalid Input!!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

            }
        }

        char loan_pass[50];
        while(two_charArray == -1){

            snprintf(response,sizeof(response),"Enter password to proceed loan:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            memset(loan_pass, 0, sizeof(loan_pass));
            recv(client_socket, loan_pass, sizeof(loan_pass) - 1, 0);
            loan_pass[sizeof(loan_pass) - 1] = '\0';

            trim(loan_pass);
            compare_two_charArray(db[index].password, loan_pass);

            if(two_charArray == -1){

                snprintf(response,sizeof(response),"Wrong credential:\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

            }else{

                db[index].cur_amount += loanAmt;

                snprintf(response,sizeof(response),"Your Current Balance: %u\n",db[index].cur_amount);
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                loan_record(index,loanAmt, db[index].tracking.total_loan_payment);
                loanTime_record(index);
                printf("\n[ Clients Data ]\n");
                printing_data();

            }

        }

    }else {

        snprintf(response,sizeof(response),"The repayment is not settled yet!!\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        loanOrRepay = -1;

    }

}

void loanTime_record(int index){

    char time_rec[1];
    get_time();

    for(int l=0; l<25; l++){
        db[index].tracking.loanTime_Rec[l] = Ctime[0].c_time[l];
    }

}

void loan_record(int index, double loanAmount, double total_loan_payment){
//Loan_Amount:$1000000-Time-Total_Payment_at_Due_Date:$1077490.00

    char display1[13]={'L','o','a','n','_','A','m','o','u','n','t',':','$'};
    char display2[28]={'-','T','o','t','a','l','_','P','a','y','m','e','n','t',
                       '_','a','t','_','D','u','e','_','D','a','t','e',':','$'};

    int index_point=0;
    int char_counter=0;

    if(loanAmount != 0){

        for(int rec=0; rec<13; rec++){
            db[index].record[db[index].tracking.recordCounter].note[rec] = display1[rec];
            index_point++;
        }
        double_to_char(loanAmount);
        char_counter = charCounting(double_to_char_data);
        for(int l=0; l<char_counter; l++){
            db[index].record[db[index].tracking.recordCounter].note[index_point] = double_to_char_data[l];
            index_point++;
        }

        get_time();
        for(int l=0; l<25; l++){
            db[index].record[db[index].tracking.recordCounter].note[index_point] = Ctime[0].c_time[l];
            index_point++;
        }

        for(int rec=0; rec<28; rec++){
            db[index].record[db[index].tracking.recordCounter].note[index_point] = display2[rec];
            index_point++;
        }

        double_to_char(total_loan_payment);
        char_counter = charCounting(double_to_char_data);
        for(int l=0; l<char_counter; l++){
            db[index].record[db[index].tracking.recordCounter].note[index_point] = double_to_char_data[l];
            index_point++;
        }

        db[index].tracking.recordCounter++;
        db[index].tracking.loanTime_rec_counter++;
    }
}

void repayment(int index, int client_socket){

    char response[1024];

    if(db[index].total_loanAmount > 0) {

        double payBack;
        double difference;
        int month;

        snprintf(response,sizeof(response),"Your Current Balance: %u\nTotal repayment for this month: %.2f\n"
                 , db[index].cur_amount, db[index].tracking.loan_payment);
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        payBack = db[index].tracking.loan_payment;

        if(total_interestRate_Minute < 11) {

            snprintf(response, sizeof(response), "Number of month/s to pay for:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket, response, strlen(response), 0);

            memset(str_input, 0, sizeof(str_input));
            recv(client_socket, str_input, sizeof(str_input) - 1, 0);
            str_input[sizeof(str_input) - 1] = '\0';

            month = charArray_to_int(str_input);

            difference = db[index].tracking.total_loan_payment - (payBack * month);

            if( difference < 0 ){

                snprintf(response, sizeof(response), "\nYour repaid amount exceed the repayment!!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket, response, strlen(response), 0);

                loanOrRepay = -1;

            }

            if (month != 0 && loanOrRepay!=-1) {

                //printf("Payment: %.2f\n",db[index].tracking.loan_payment * month);
                //printf("PayBack: %.2f\n",payBack * month);

                difference = (db[index].tracking.loan_payment * month) - (payBack * month);

                if ( double_var_checking(difference) < tolerance ) {

                    db[index].tracking.total_loan_payment -= payBack * month;
                    db[index].tracking.loan_payment -= payBack * month;
                    db[index].cur_amount -= payBack * month;

                    snprintf(response, sizeof(response), "Remaining Payment: %.2f\nRemaining Balance: %u\n",
                             db[index].tracking.total_loan_payment, db[index].cur_amount);
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket, response, strlen(response), 0);

                    if(db[index].tracking.total_loan_payment <= 0){

                        db[index].tracking.repaid_switch = 0;
                        db[index].total_loanAmount = 0;
                        db[index].tracking.total_loan_payment = 0;
                        db[index].tracking.loan_payment = db[index].tracking.total_loan_payment;

                        snprintf(response, sizeof(response), "Repaid Completely..Thank You!!\n");
                        response[sizeof(response) - 1] = '\0';
                        send(client_socket, response, strlen(response), 0);

                    }

                    repayment_record(index, payBack * month, db[index].tracking.total_loan_payment);
                    db[index].tracking.repaid_switch = month;

                    printf("\n[ Clients Data ]\n");
                    printing_data();

                } else if ( difference < 0 ) {

                    snprintf(response, sizeof(response), "\nYour repaid amount exceed the repayment!!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket, response, strlen(response), 0);

                    loanOrRepay = -1;

                } else {

                    snprintf(response, sizeof(response), "\nInsufficient Amount!!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket, response, strlen(response), 0);

                    loanOrRepay = -1;

                }

            }

        }else {

            double final_payBack;

            snprintf(response, sizeof(response), "Input for your final repayment:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket, response, strlen(response), 0);

            memset(str_input, 0, sizeof(str_input));
            recv(client_socket, str_input, sizeof(str_input) - 1, 0);
            str_input[sizeof(str_input) - 1] = '\0';

            final_payBack = strtod(str_input,NULL); // to avoid slight difference when changing str_input to double dataType

            difference = db[index].tracking.loan_payment-final_payBack;
            if ( double_var_checking(difference) < tolerance ) {

                db[index].tracking.total_loan_payment -= final_payBack;
                db[index].tracking.loan_payment -= final_payBack;
                db[index].cur_amount -= final_payBack;

                if(db[index].tracking.total_loan_payment <= 0){

                    db[index].tracking.repaid_switch = 0;
                    db[index].total_loanAmount = 0;
                    db[index].tracking.total_loan_payment = 0;
                    db[index].tracking.loan_payment = db[index].tracking.total_loan_payment;

                    snprintf(response, sizeof(response), "Repaid Completely..Thank You!!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket, response, strlen(response), 0);

                }

                repayment_record(index, final_payBack, db[index].tracking.total_loan_payment);
                printf("\n[ Clients Data ]\n");
                printing_data();

            }else if(difference < 0){

                snprintf(response, sizeof(response), "\nInvalid Amount!!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket, response, strlen(response), 0);

                loanOrRepay = -1;

            }else{

                snprintf(response, sizeof(response), "\nInvalid Amount!!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket, response, strlen(response), 0);

                loanOrRepay = -1;

            }

        }

    }else{

        snprintf(response, sizeof(response), "\nYou have no loan yet at our bank!!\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket, response, strlen(response), 0);

        loanOrRepay = -1;

    }

}

void repayment_record(int index, double repaid_amount, double remaining_payment){
//Repaid:$0000-Remaining_Payment:$000-Time

    char display1[8]={'R','e','p','a','i','d',':','$'};
    char display2[20]={'-','R','e','m','a','i','n','i','n','g','_',
                       'P','a','y','m','e','n','t',':','$'};
    int index_point=0;
    int char_counter=0;

    if(repaid_amount != 0){

        for(int rec=0; rec<8; rec++){
            db[index].record[db[index].tracking.recordCounter].note[rec] = display1[rec];
            index_point++;
        }

        double_to_char(repaid_amount);
        char_counter = charCounting(double_to_char_data);
        for(int r=0; r<char_counter; r++){
            db[index].record[db[index].tracking.recordCounter].note[index_point] = double_to_char_data[r];
            index_point++;
        }

        for(int rec=0; rec<20; rec++){
            db[index].record[db[index].tracking.recordCounter].note[index_point] = display2[rec];
            index_point++;
        }

        double_to_char(remaining_payment);
        char_counter = charCounting(double_to_char_data);
        for(int r=0; r<char_counter; r++){
            db[index].record[db[index].tracking.recordCounter].note[index_point] = double_to_char_data[r];
            index_point++;
        }

        get_time();
        for(int r=0; r<25; r++){
            db[index].record[db[index].tracking.recordCounter].note[index_point] = Ctime[0].c_time[r];
            db[index].tracking.repaidTime_Rec[r] = Ctime[0].c_time[r];
            index_point++;
        }

        db[index].tracking.recordCounter++;

    }

}

void get_limit_amount(int index){

    // 1 for personal and 2 for business
    int acc_level = db[index].acc_level;
    char pOrb =db[index].pOrb[0];
    int p_or_b = 0;
    if(pOrb == 'p'){
        p_or_b=1;
    } else{
        p_or_b=2;
    }

    switch (acc_level) {
        case 1:
            if(p_or_b==1){
                trans_limit=100000;
            } else{
                trans_limit=1000000;
            }
            break;
        case 2:
            if(p_or_b==1){
                trans_limit=500000;
            } else{
                trans_limit=5000000;
            }
            break;
        case 3:
            if(p_or_b==1){
                trans_limit=1000000;
            } else{
                trans_limit=10000000;
            }
            break;
        default:
            break;
    }

}

void get_time(){

    time_t tm;
    time(&tm);

    FILE *fptr = fopen("myTime.txt","w");
    fprintf(fptr,"%s", ctime(&tm));

    fclose(fptr);

    int t_index=0;
    int time_space_counter=0;

    Ctime[0].c_time[t_index]='-';
    t_index++;

    FILE *fptr2 = fopen("myTime.txt","r");
    char c = fgetc(fptr2);

    while (!feof(fptr2)){

        if( c==' '){

            time_space_counter++;

            if(time_space_counter == 1){
                Ctime[0].c_time[t_index]='!';
                c = fgetc(fptr2);
                t_index++;
            }else if(time_space_counter==4){
                Ctime[0].c_time[t_index]='@';
                c = fgetc(fptr2);
                t_index++;
            }else{
                Ctime[0].c_time[t_index]='-';
                c = fgetc(fptr2);
                t_index++;
            }

        }else{

            Ctime[0].c_time[t_index]=c;
            c = fgetc(fptr2);
            t_index++;

        }
    }
}

void get_current_day(){

    char get_currentDay[2];
    get_time();
    get_currentDay[0]=Ctime[0].c_time[9];
    get_currentDay[1]=Ctime[0].c_time[10];
    unsigned int current_day_to_transfer=charArray_to_unsigned_fun(get_currentDay);
    currentDay=current_day_to_transfer;
}

void integer_to_char(unsigned int value){

    FILE *fptr = fopen("convert.txt","w");

    if(fptr==NULL){
        printf("file opening error at integer_to_char:\n");
    } else{
        fprintf(fptr,"%u",value);
    }
    fclose(fptr);

    FILE *fptr2 = fopen("convert.txt","r");
    fscanf(fptr2,"%s",&int_to_char_array_data[0]);

}

unsigned int charArray_to_unsigned_fun(char charArray[]){

    unsigned int data=0;
    FILE *fptr = fopen("convert.txt","w");

    if(fptr==NULL){
        printf("file opening error at charArray_to_unsigned_fun:\n");
    } else{
        fprintf(fptr,"%s",charArray);
    }
    fclose(fptr);

    FILE *fptr2 = fopen("convert.txt","r");
    fscanf(fptr2,"%u",&data);

    return data;
}

int charArray_to_int(char charArray[]){

    int data=0;
    FILE *fptr = fopen("convert.txt","w");

    if(fptr==NULL){
        printf("file opening error at charArray_to_int:\n");
    } else{
        fprintf(fptr,"%s",charArray);
    }
    fclose(fptr);

    FILE *fptr2 = fopen("convert.txt","r");
    fscanf(fptr2,"%d",&data);

    return data;
}

int charArray_to_llu(char charArray[]){

    unsigned long long int data=0;
    FILE *fptr = fopen("convert.txt","w");

    if(fptr==NULL){
        printf("file opening error at charArray_to_llu:\n");
    } else{
        fprintf(fptr,"%s",charArray);
    }
    fclose(fptr);

    FILE *fptr2 = fopen("convert.txt","r");
    fscanf(fptr2,"%llu",&data);

    return data;
}

void double_to_char(double value){

    FILE *fptr = fopen("convert.txt","w");

    if(fptr==NULL){
        printf("file opening error at double_to_char:\n");
    } else{
        fprintf(fptr,"%.2f",value);
    }
    fclose(fptr);

    FILE *fptr2 = fopen("convert.txt","r");
    fscanf(fptr2,"%s",&double_to_char_data[0]);

}

double double_var_checking(double value){

    if(value < 0){
        return -value;
    }else{
        return value;
    }

}

void update_information(int index, int client_socket){

    char message[4096];

    snprintf(message, sizeof(message), "\nName: %s\nEmail: %s\nPhone: %llu\nNRC: %s\nAddress: %s\nPassword: %s\nAcc Type: %s\nAcc Level: %d\n",
             db[index].name,db[index].email,db[index].phNumber,db[index].nrc,db[index].address,db[index].password,db[index].pOrb,db[index].acc_level);
    message[sizeof(message) - 1] = '\0';
    send(client_socket, message, strlen(message), 0);

    char response[1024];
    int updateInfo_option=0;
    char update_email[50];
    char update_name[50];
    unsigned long long int update_phone;
    char update_nrc[50];
    char update_address[50];
    char update_password[50];

    snprintf(response,sizeof(response),"\nPress 1 to update name:\nPress 2 to update email:\nPress 3 to update phone:\n"
                                       "Press 4 to update NRC:\nPress 5 to update address:\nPress 6 to update password:\n"
                                       "Press 7 to update account level:\nPress 8 to back user sector:\n");
    response[sizeof(response) - 1] = '\0';
    send(client_socket,response,strlen(response),0);

    memset(str_input, 0, sizeof(str_input));
    recv(client_socket, str_input, sizeof(str_input) - 1, 0);
    str_input[sizeof(str_input) - 1] = '\0';

    updateInfo_option = charArray_to_int(str_input);

    if (updateInfo_option == 1){

        snprintf(response,sizeof(response),"Enter new name:");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        memset(update_name, 0, sizeof(update_name));
        recv(client_socket, update_name, sizeof(update_name) - 1, 0);
        update_name[sizeof(update_name) - 1] = '\0';

        trim(update_name);

        proceed_or_back(client_socket, index);

        if(option_key == 1){
            copy_two_char_array(db[index].name,update_name);
        }

    }else if (updateInfo_option == 2){

        gmailValid = -1;
        while(gmailValid == -1){

            snprintf(response,sizeof(response),"Enter new email:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            memset(update_email, 0, sizeof(update_email));
            recv(client_socket, update_email, sizeof(update_email) - 1, 0);
            update_email[sizeof(update_email) - 1] = '\0';

            trim(update_email);
            gmailValidation(update_email);

            if(gmailValid != -1){

                emailExist = -1;
                emailExistChecking(root,update_email);

                if(emailExist == -1){

                    proceed_or_back(client_socket, index);
                    if(option_key == 1){
                        copy_two_char_array(db[index].email,update_email);
                    }
                    gmailValid = 1;

                }else{
                    snprintf(response,sizeof(response),"Your gmail already taken!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);
                    gmailValid=-1;
                }

            }else{
                snprintf(response,sizeof(response),"Invalid gmail format!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);
                gmailValid=-1;
            }

        }

    }else if (updateInfo_option == 3){

        phone_valid=-1;
        while(phone_valid!=1){

            dataType_Valid=-1;
            while(dataType_Valid==-1){

                snprintf(response,sizeof(response),"Enter new phone number:");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                memset(str_input, 0, sizeof(str_input));
                recv(client_socket, str_input, sizeof(str_input) - 1, 0);
                str_input[sizeof(str_input) - 1] = '\0';

                trim(str_input);
                int_checker(str_input);

            }

            existence=0;
            update_phone = charArray_to_llu(str_input);
            phone_validation(root,update_phone);

            if(existence==0){

                proceed_or_back(client_socket, index);

                if(option_key == 1){
                    db[index].phNumber = update_phone;
                    phone_valid=1;
                }

            }else{
                snprintf(response,sizeof(response),"Your phone is already registered!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);
            }

        }

    }else if (updateInfo_option == 4){

        nrc_valid=-1;
        while(nrc_valid==-1){

            snprintf(response,sizeof(response),"Enter NRC:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            memset(update_nrc, 0, sizeof(update_nrc));
            recv(client_socket, update_nrc, sizeof(update_nrc) - 1, 0);
            update_nrc[sizeof(update_nrc) - 1] = '\0';

            trim(update_nrc);
            nrcValidation(update_nrc);

            if(nrc_valid == -1){
                snprintf(response,sizeof(response),"This NRC Format is unavailable!!!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

            }else{
                proceed_or_back(client_socket, index);
                if(option_key == 1){
                    copy_two_char_array(db[index].nrc,update_nrc);
                }
            }

        }
    }else if (updateInfo_option == 5){

        snprintf(response,sizeof(response),"Enter Address:");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        memset(update_address, 0, sizeof(update_address));
        recv(client_socket, update_address, sizeof(update_address) - 1, 0);
        update_address[sizeof(update_address) - 1] = '\0';

        trim(update_address);
        proceed_or_back(client_socket, index);

        if(option_key == 1){
            copy_two_char_array(db[index].address,update_address);
        }

    }else if (updateInfo_option == 6){

        char password_again[50];

        strongPass=-1;
        while(strongPass==-1){

            snprintf(response,sizeof(response),"Enter Password:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            memset(update_password, 0, sizeof(update_password));
            recv(client_socket, update_password, sizeof(update_password) - 1, 0);
            update_password[sizeof(update_password) - 1] = '\0';

            trim(update_password);

            existence = 0;
            passwordExistChecking(root,update_password);

            if(existence == 1){

                snprintf(response,sizeof(response),"This Password registered already!\nPlease Try Again!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);
                strongPass=-1;

            }else{

                strongPassword(update_password);
                if(strongPass==-1){

                    snprintf(response,sizeof(response),"Password format is too weak!\nPlease Try Again!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);

                }else{

                    proceed_or_back(client_socket, index);
                    if(option_key == 1){

                        two_charArray = -1;
                        while(two_charArray==-1){

                            snprintf(response,sizeof(response),"Enter your new password again:");
                            response[sizeof(response) - 1] = '\0';
                            send(client_socket,response,strlen(response),0);

                            memset(password_again, 0, sizeof(password_again));
                            recv(client_socket, password_again, sizeof(password_again) - 1, 0);
                            password_again[sizeof(password_again) - 1] = '\0';

                            trim(password_again);

                            compare_two_charArray(update_password,password_again);

                            if(two_charArray != -1){
                                copy_two_char_array(db[index].password,update_password);

                            }else{
                                two_charArray=-1;
                            }
                        }
                    }
                }
            }
        }
    }else if (updateInfo_option == 7){

        snprintf(response,sizeof(response),">> Pay $36 to upgrade fee <<\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        unsigned int charges;
        int upgrade_valid=-1;

        dataType_Valid=-1;
        while(upgrade_valid == -1) {

            snprintf(response,sizeof(response),"Enter charges to upgrade:");
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

            memset(str_input, 0, sizeof(str_input));
            recv(client_socket, str_input, sizeof(str_input) - 1, 0);
            str_input[sizeof(str_input) - 1] = '\0';

            trim(str_input);
            int_checker(str_input);

            if (dataType_Valid != -1) {

                charges = charArray_to_int(str_input);

                if (charges == 36 && db[index].acc_level < 3) {

                    proceed_or_back(client_socket, index);
                    if (option_key == 1) {

                        upgrade_valid = 1;
                        db[index].acc_level += 1;
                        db[index].cur_amount -= 36;
                        get_limit_amount(index);
                        db[index].transAmoLimitPerDay = trans_limit;
                        upgrade_accLvl_record(index);
                        printf("\n[ Clients Data ]\n");
                        printing_data();

                    }

                } else if (charges != 36) {

                    upgrade_valid = -1;
                    snprintf(response,sizeof(response),"Invalid Amount!!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);

                } else if (db[index].acc_level >= 3) {

                    snprintf(response,sizeof(response),"Your upgraded ultimately!! No higher level anymore!!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);

                    back_option(client_socket, index);

                } else {

                    snprintf(response,sizeof(response),"Invalid Option!!\n");
                    response[sizeof(response) - 1] = '\0';
                    send(client_socket,response,strlen(response),0);

                }

            }else{
                upgrade_valid = -1;
            }
        }

    }else if (updateInfo_option == 8){

        user_sector(client_socket, index);

    }else{

        snprintf(response,sizeof(response),"Invalid Option!!\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        back_option(client_socket, index);

    }

    back_option(client_socket, index);

}

void upgrade_accLvl_record(int index){
//AccLevel_Upgraded_to_Level:%d_Paid:$_Time

    char display1[27]={'A','c','c','L','e','v','e','l','_','U','p','g',
                       'r','a','d','e','d','_','t','o','_','L','e',
                       'v','e','l',':'};
    char display2[7]={'-','P','a','i','d',':','$'};
    char display3[25]={'-','T','r','a','n','s','a','c','t','i','o',
                       'n','L','i','m','i','t','P','e','r',
                       'D','a','y',':','$'};

    int index_point=0;
    int char_counter=0;

    for(int rec=0; rec<27; rec++){
        db[index].record[db[index].tracking.recordCounter].note[rec] = display1[rec];
        index_point++;
    }

    integer_to_char(db[index].acc_level);
    db[index].record[db[index].tracking.recordCounter].note[index_point] = int_to_char_array_data[0];
    index_point++;

    for(int rec=0; rec<7; rec++){
        db[index].record[db[index].tracking.recordCounter].note[index_point] = display2[rec];
        index_point++;
    }

    integer_to_char(36);
    db[index].record[db[index].tracking.recordCounter].note[index_point] = int_to_char_array_data[0];
    index_point++;
    db[index].record[db[index].tracking.recordCounter].note[index_point] = int_to_char_array_data[1];
    index_point++;

    for(int rec=0; rec<25; rec++){
        db[index].record[db[index].tracking.recordCounter].note[index_point] = display3[rec];
        index_point++;
    }

    integer_to_char(db[index].transAmoLimitPerDay);
    unsigned int integer_count = charCounting(int_to_char_array_data);
    for(int rec=0; rec<integer_count; rec++){
        db[index].record[db[index].tracking.recordCounter].note[index_point] = int_to_char_array_data[rec];
        index_point++;
    }

    get_time();
    for(int rec=0; rec<25; rec++){
        db[index].record[db[index].tracking.recordCounter].note[index_point] = Ctime[0].c_time[rec];
        index_point++;
    }

    db[index].tracking.recordCounter++;

}

void history(int index, int client_socket){

    char response[1024];
    int history=0;
    int initial_point=db[index].tracking.recordCounter;

    if(initial_point != 0){
        for(history=initial_point; history>=0; history--){

            snprintf(response,sizeof(response),"%s\n",db[index].record[history].note);
            response[sizeof(response) - 1] = '\0';
            send(client_socket,response,strlen(response),0);

        }
    }else{

        snprintf(response,sizeof(response),"\nNo record yet !!!\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

    }

    back_option(client_socket, index);

}

void proceed_or_back(int client_socket, int index){

    char response[1024];
    int opt_switch = -1;
    dataType_Valid = -1;

    while(opt_switch == -1){

        snprintf(response,sizeof(response),"Press 1 to proceed:/ Press 2 to back user sector:");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        memset(str_input, 0, sizeof(str_input));
        recv(client_socket, str_input, sizeof(str_input) - 1, 0);
        str_input[sizeof(str_input) - 1] = '\0';

        trim(str_input);
        int_checker(str_input);

        if(dataType_Valid != -1){

            opt_switch = charArray_to_int(str_input);

            if(opt_switch == 1){
                option_key = 1;

            }else if(opt_switch == 2){
                user_sector(client_socket, index);

            }else{

                snprintf(response,sizeof(response),"Invalid Option!!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                opt_switch = -1;

            }
        }else{
            opt_switch = -1;
        }
    }
}

void back_option(int client_socket, int index){

    char response[1024];
    int opt_switch = -1;
    dataType_Valid = -1;

    while(opt_switch == -1){

        snprintf(response,sizeof(response),"\nPress 1 Back to User Sector:/ Press 2 Exit:\n");
        response[sizeof(response) - 1] = '\0';
        send(client_socket,response,strlen(response),0);

        memset(str_input, 0, sizeof(str_input));
        recv(client_socket, str_input, sizeof(str_input) - 1, 0);
        str_input[sizeof(str_input) - 1] = '\0';

        trim(str_input);
        int_checker(str_input);

        if(dataType_Valid != -1){

            opt_switch = charArray_to_int(str_input);

            if(opt_switch == 1){
                user_sector(client_socket, index);

            }else if(opt_switch == 2){

                snprintf(response,sizeof(response),"***>>> Thanks For Being Our Customer <<<***\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                recordData_to_file(root);
                exit(0);

            }else{

                snprintf(response,sizeof(response),"Invalid Option!!\n");
                response[sizeof(response) - 1] = '\0';
                send(client_socket,response,strlen(response),0);

                opt_switch = 0;

            }
        }else{
            opt_switch = 0;
        }

    }

}

void print_t_file(struct Node *root){

    if(root != NULL){
        FILE *fptr = fopen("bankData.txt","a");

        print_t_file(root->left);
        fprintf(fptr,"%u%c%s%c%s%c%llu%c%s%c%s%c%s%c%s%c%u%c%.0f%c%u%c%d",root->seed->id,' ',root->seed->name,' ',root->seed->email,' ',
                root->seed->phNumber,' ',root->seed->nrc,' ',root->seed->address,' ',root->seed->password ,' ',root->seed->pOrb ,' ',
                root->seed->cur_amount,' ',root->seed->total_loanAmount,' ',root->seed->transAmoLimitPerDay,' ',root->seed->acc_level);

        for(int gcc=0; gcc<root->seed->tracking.recordCounter ; gcc++){
            fprintf(fptr,"%c",'\n');
            fprintf(fptr," %s",root->seed->record[gcc].note);
        }

        fprintf(fptr,"%c%c",'\n','\n');
        print_t_file(root->right);
    }
}

void print_trackData_t_file(struct Node *root){

    if(root != NULL){
        FILE *fptr2 = fopen("bankInfo_tracking.txt","a");

        print_trackData_t_file(root->left);
        fprintf(fptr2,"%d%c%u%c%u%c%u%c%u%c%u%c%d%c%d",root->seed->tracking.recordCounter,' ',root->seed->tracking.totalTransfer,' ',
                root->seed->tracking.totalReceive,' ',root->seed->tracking.totalCashOut,' ',root->seed->tracking.trans_perDay,' ',
                root->seed->tracking.cashOut_perDay,' ',root->seed->tracking.loanTime_rec_counter,' ',root->seed->tracking.repaid_switch);
        fprintf(fptr2,"%c",'\n');
        print_trackData_t_file(root->right);
    }
}

void print_loanData_t_file(struct Node *root){
    if(root != NULL){
        FILE *fptr3 = fopen("loanPayment_Record.txt","a");

        print_loanData_t_file(root->left);
        fprintf(fptr3,"%.2f",root->seed->tracking.loan_payment);
        fprintf(fptr3," ");
        fprintf(fptr3,"%.2f",root->seed->tracking.total_loan_payment);
        fprintf(fptr3," ");
        fprintf(fptr3,"%s",root->seed->tracking.loanTime_Rec);
        fprintf(fptr3," ");
        fprintf(fptr3,"%s",root->seed->tracking.repaidTime_Rec);
        fprintf(fptr3,"\n");
        print_loanData_t_file(root->right);
    }
}

void recordData_to_file(struct Node *root){

    FILE *fptr = fopen("bankData.txt","w");
    if(fptr==NULL){
        printf("Cannot open file to record!!\n");
    }
    print_t_file(root);
    fclose(fptr);

    FILE *fptr2 = fopen("bankInfo_tracking.txt","w");
    if(fptr2==NULL){
        printf("Cannot open file to record!!\n");
    }
    print_trackData_t_file(root);
    fclose(fptr2);

    FILE *fptr3 = fopen("loanPayment_Record.txt","w");
    if(fptr3==NULL){
        printf("Cannot open file to record!!\n");
    }
    print_loanData_t_file(root);
    fclose(fptr3);
}


#endif //FINAL_ASSIGNMENT_BANK_PROGRAM_H

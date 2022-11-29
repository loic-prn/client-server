
/*
 * SPDX-FileCopyrightText: 2022 John Samuel
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/* 
 * Code pour traiter différents types d'opérations mathématiques
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "json.h"
#include "operations.h"
#include "common.h"

int calculator(struct Calc *c, float* output){
  if (c->operation == '+'){
    *output = c->nums[0] + c->nums[1];
  }
  else if (c->operation == '-'){
    *output = c->nums[0] - c->nums[1];
  }
  else if (c->operation == '*'){
    *output = c->nums[0] * c->nums[1];
  }
  else if (c->operation == '/'){
    *output = c->nums[0] / c->nums[1];
  }
  else{
    return -3; // wrong operation
  }
  return EXIT_SUCCESS;
}

int calcul(int client_socket_fd, char *data){
  float result = 0.0;
  struct Calc *c = malloc(sizeof(struct Calc));
  c->operation = '%';
  c->nums[0] = 0.0;
  c->nums[1] = 0.0;
  if(get_calcul(data, c)){
    create_error_message(data, "parsing input");
  }

  int status = calculator(c, &result);
  char tmp[20];

  switch(status){
    case -1:
      create_error_message(data, "invalid operation");
      break;
    case -2:
      create_error_message(data, "Invalid input - couldn't parse data");
      break;
    case -3:
      create_error_message(data, "Impossible operation");
      break;
    default:
    printf("result %f\n", result);
      if(sprintf(tmp, "%f", result) < 0){
        create_error_message(data, "Internal server error");
      }
      else {
        set_message(tmp, data);
      }
  }
  free(c);

  if(write(client_socket_fd, (void *)data, strlen(data)) < 0){
    perror("error sending datas");
    return EXIT_FAILURE;
  } 

  return EXIT_SUCCESS;
}

int mini(int client_socket_fd, char *data){
  int n;
  int count=0;
  int size_tab=0;
  int tab_int[1024];
  int smallest_val;

  char *str = data;

  unsigned int start_delimiter = strlen(FIRST_JSON_PART) + 3 + strlen(ARRAY_JSON_PART);
  memset(&str[strlen(str) - 2], 0, sizeof(char)*2);

  while (1){
    char *token = strtok(&str[start_delimiter], ",");
    if (token == NULL){
      break;
    }

    sscanf(token,"\"%d\"",&n);

    if(count==0){
      size_tab=n;
    }else{
      tab_int[count-1]=n;
    }


    smallest_val = tab_int[0];

    for(int i=0;i<size_tab;i++){
      if(tab_int[i]<smallest_val){
        smallest_val=tab_int[i];
      }
    }

    count++;
    start_delimiter+=strlen(token)+1;
  }

  memset(data, 0, sizeof(char)*1024);
  char int_to_str[10];
  char buff[1024];
  sprintf(int_to_str, "%d", smallest_val);
  strcat(buff,"Smallest value is : ");
  strcat(buff,int_to_str);
  printf("%s\n",buff);

  char msg[1024];
  strcpy(msg,buff);

  create_ok_message(data, msg);
    if(write(client_socket_fd, (void *)data, strlen(data)) < 0){
      printf("[/!\\] An error occured while sending a message");
    }
  return EXIT_SUCCESS;
}

int maxi(int client_socket_fd, char *data){
  int n;
  int count=0;
  int size_tab=0;
  int tab_int[1024];
  int biggest_val;

  char *str = data;

  unsigned int start_delimiter = strlen(FIRST_JSON_PART) + 3 + strlen(ARRAY_JSON_PART);
  memset(&str[strlen(str) - 2], 0, sizeof(char)*2);

  while (1){
    char *token = strtok(&str[start_delimiter], ",");
    if (token == NULL){
      break;
    }

    sscanf(token,"\"%d\"",&n);

    if(count==0){
      size_tab=n;
    }else{
      tab_int[count-1]=n;
    }


    biggest_val = tab_int[0];

    for(int i=0;i<size_tab;i++){
      if(tab_int[i]>biggest_val){
        biggest_val=tab_int[i];
      }
    }

    count++;
    start_delimiter+=strlen(token)+1;
  }

  memset(data, 0, sizeof(char)*1024);
  char int_to_str[10];
  char buff[1024];
  sprintf(int_to_str, "%d", biggest_val);
  strcat(buff,"Biggest value is : ");
  strcat(buff,int_to_str);
  printf("%s\n",buff);

  char msg[1024];
  strcpy(msg,buff);

  create_ok_message(data, msg);
    if(write(client_socket_fd, (void *)data, strlen(data)) < 0){
      printf("[/!\\] An error occured while sending a message");
    }
  return EXIT_SUCCESS;
}

int avg(int client_socket_fd, char* data){
  int n;
  int number=0;
  int count=0;
  int buff=0;

  float avg;

  char* str = data;
  char temp[1024];
  char tempon[1024];

  unsigned int start_delimiter = strlen(FIRST_JSON_PART) + 3 + strlen(ARRAY_JSON_PART);
  memset(&str[strlen(str) - 2], 0, sizeof(char)*2);

  while (1){
    char *token = strtok(&str[start_delimiter], ",");
    if (token == NULL){
      break;
    }

    sscanf(token,"\"%d\"",&n);

    if(count==0){
      number=n;
    }else{
      buff = buff + n;
    }


    count++;
    start_delimiter+=strlen(token)+1;
  }

  avg = buff/number;

  sprintf(temp, "%f", avg);
  strcat(tempon,"Average is : ");
  strcat(tempon,temp);

  char msg[1024];
  strcpy(msg,tempon);

  create_ok_message(data, msg);
    if(write(client_socket_fd, (void *)data, strlen(data)) < 0){
      printf("[/!\\] An error occured while sending a message");
    }
    memset(tempon, 0, sizeof(char)*1024);
  return EXIT_SUCCESS;
}
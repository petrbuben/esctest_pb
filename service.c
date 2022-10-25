/*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *                           ALL RIGHTS RESERVED                              *
 *                                                                            *
 ******************************************************************************
 */

/**
 *  @file service.c
 *  @author Petr Buben
 *  @brief implement functions
 *
 * Copyright 2022 s.r.o., All rights reserved.
 */

  #include "service.h"
  #include "TriggerEvent.h"
  #include "Event.h"
  #include "WriteAppendEvents.h"
  #include <stdio.h>
  #include <stdlib.h>
  #include <pthread.h>
  #include <stdalign.h>

  #include <sys/timeb.h>

  extern pthread_mutex_t lock;
  extern alignas(64) Event_t mainEventLoop_Events_record_array[EV_RECORD_SIZE];
  extern uint8_t ev_arr_index;
  extern alignas(64) Event_t TriggerEvent_ev;
  extern const char* Queue_TypeEv_e[];
  extern struct timeb tstart, tend;
  extern float rate_per_type;

  void mainEventLoop_Raise_critical(Type_et type, char* file, uint8_t line)
  {
        pthread_mutex_lock(&lock);
        //record and exit
        struct timeb t;
        TriggerEvent_TrigEvSim(t, E_MEDIUM_SEVERITY, type, file, line, "0");
        mainEventLoop_Events_record_array[ev_arr_index] = TriggerEvent_ev;
        printf("Record array %s in file %s",
               Queue_TypeEv_e[mainEventLoop_Events_record_array[ev_arr_index].typ_e],
               mainEventLoop_Events_record_array[ev_arr_index].file_loc);
        printf("\n\nE_MEDIUM_SEVERITY event %s, exiting.........\n", Queue_TypeEv_e[type]);
        system("pause");

        return;
  }


  float figure_rate()
  {
        int tdiff = 0;
        float rate_per_type = 0.;
        //struct timeb tend;
        //ftime(&tend);

        tdiff = (int) (1000.0 * (tend.time - tstart.time) + tend.millitm - tstart.millitm);
            printf("Time difference %d ms\n", tdiff);
            //count / tdiff
        rate_per_type =
            (float)1000*(1+mainEventLoop_Events_record_array[ev_arr_index].type_event_count)
            /(float)tdiff;

        printf("rate for event type %s is %.3f per sec\n",
                Queue_TypeEv_e[mainEventLoop_Events_record_array[ev_arr_index].typ_e],
                rate_per_type);

        return rate_per_type;
  }

  void Record_rate_count_write_file()
  {
        mainEventLoop_Events_record_array[ev_arr_index].type_event_rate = rate_per_type;

        //increase this event count
        ++mainEventLoop_Events_record_array[ev_arr_index].type_event_count;
        printf("Record array %s",
               Queue_TypeEv_e[mainEventLoop_Events_record_array[ev_arr_index].typ_e]);

        //append file tbd
        WriteAppendEvents_write_file(&mainEventLoop_Events_record_array[ev_arr_index]);

        return;
  }


  /*
      //delete event record file
      if(0 == remove(WA_FILENAME)) //delete file
      {
         printf("File deleted successfully\n");
      }
      else
      {
          printf("No file to delete\n");
          //raise critical, exit
      }
  */

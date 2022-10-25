/*
 ******************************************************************************
 *                                                                            *
 *                                                                            *
 *                           ALL RIGHTS RESERVED                              *
 *                                                                            *
 ******************************************************************************
 */

/**
 *  @file WriteAppendEvents.c
 *  @author Petr Buben
 *  @brief Write append readable events file
 *
 * Copyright s.r.o., All rights reserved.
 */

 #include "WriteAppendEvents.h"
 #include "Event.h"
 #include "Modules.h"
 #include "service.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <stdalign.h>

 //static const Modules_Id_e m_eModuleId = E_MODULES_ID_WRITEAPPENDEVENTS;


extern alignas(64) const char* Queue_SevEv_e[];
extern alignas(64) const char* Queue_TypeEv_e[];

char *time_stamp(){

char *timestamp = (char *)malloc(sizeof(char) * 16);
time_t ltime;
ltime=time(NULL);
struct tm *tm;
tm=localtime(&ltime);

sprintf(timestamp,"%04d%02d%02d%02d%02d%02d", tm->tm_year+1900, tm->tm_mon,
    tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
return timestamp;
}

void WriteAppendEvents_write_file(Event_t *ev)
{
     FILE *ofptr;
     ofptr = fopen(WA_FILENAME,"a");

     if( NULL == ofptr)
     {
         //raise critical error
         printf("\nError: unable to open the file\n");
         mainEventLoop_Raise_critical(E_FILE, __FILE__, __LINE__);
         exit(-1);
     }

     fprintf(ofptr,
    "Timestamp:%s\nSeverity:%s Type:%s Count:%d Rate:%.3f\nFile:%s Line:%u\nUser Par:%s\n",
    time_stamp(), Queue_SevEv_e[ev->sev_e], Queue_TypeEv_e[ev->typ_e], ev->type_event_count,
    ev->type_event_rate, ev->file_loc, ev->line_num, ev->user_param);
    if(2 == ev->sev_e)
    {
         fprintf(ofptr,"Medium severity exception, resetting\n\n");
    }

    fclose (ofptr);

    return;
}


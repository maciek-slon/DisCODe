/*
** tut_2_3.c
**
**
** Started on  Mon Nov 27 17:10:52 2006 Arne Caspari
** Last update Sun Feb  4 18:40:13 2007 Arne Caspari
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unicap.h>

#define MAX_DEVICES 64
#define MAX_FORMATS 64
#define MAX_PROPERTIES 64

unicap_handle_t
open_device ()
{
  int dev_count;
  int status = STATUS_SUCCESS;
  unicap_device_t devices[MAX_DEVICES];
  unicap_handle_t handle;
  int d = -1;

  for (dev_count = 0; SUCCESS (status) && (dev_count < MAX_DEVICES);
       dev_count++)
    {
      // (1)
      status =
	unicap_enumerate_devices (NULL, &devices[dev_count], dev_count);
      if (SUCCESS (status))
	{
	  printf ("%d: %s\n", dev_count, devices[dev_count].identifier);
	}
      else
	{
	  break;
	}
    }

  if (dev_count == 0)
    {
      // no device selected
      return NULL;
    }


  while ((d < 0) || (d >= dev_count))
    {
      printf ("Open Device f: ");
      scanf ("%d", &d);
    }

  unicap_open (&handle, &devices[d]);

  return handle;
}

void
set_format (unicap_handle_t handle)
{
  unicap_format_t formats[MAX_FORMATS];
  int format_count;
  unicap_status_t status = STATUS_SUCCESS;
  int f = -1;

  for (format_count = 0; SUCCESS (status) && (format_count < MAX_FORMATS);
       format_count++)
    {
      status = unicap_enumerate_formats (handle, NULL, &formats[format_count],
					 format_count);
      if (SUCCESS (status))
	{
	  printf ("%d: %s\n", format_count, formats[format_count].identifier);
	}
      else
	{
	  break;
	}
    }

  if (format_count == 0)
    {
      // no video formats
      return;
    }

  while ((f < 0) || (f >= format_count))
    {
      printf ("Use Format: ");
      scanf ("%d", &f);
    }

  if (formats[f].size_count)
    {
      int i;
      int s = -1;

      for (i = 0; i < formats[f].size_count; i++)
	{
	  printf ("%d: %dx%d\n", i, formats[f].sizes[i].width,
		  formats[f].sizes[i].height);
	}

      while ((s < 0) || (s >= formats[f].size_count))
	{
	  printf ("Select Size: ");
	  scanf ("%d", &s);
	}

      formats[f].size.width = formats[f].sizes[s].width;
      formats[f].size.height = formats[f].sizes[s].height;
    }

  if (!SUCCESS (unicap_set_format (handle, &formats[f])))
    {
      fprintf (stderr, "Failed to set the format!\n");
      exit (-1);
    }
}

void set_menu_property( unicap_handle_t handle )
{
   unicap_property_t properties[MAX_PROPERTIES];
   int property_count;
   int menu_ppty_count;
   unicap_status_t status = STATUS_SUCCESS;
   int p = -1;
   int item = -1;
   int i;

   for( property_count = menu_ppty_count = 0; SUCCESS( status ) && ( property_count < MAX_PROPERTIES ); property_count++ )
   {
      status = unicap_enumerate_properties( handle, NULL, &properties[menu_ppty_count], property_count );
      if( SUCCESS( status ) )
      {
	 if( properties[menu_ppty_count].type == UNICAP_PROPERTY_TYPE_MENU )
	 {
	    printf( "%d: %s\n", menu_ppty_count, properties[menu_ppty_count].identifier );
	    menu_ppty_count++;
	 }
      }
      else
      {
	 break;
      }
   }

   if( menu_ppty_count == 0 )
   {
      // no menu properties
      return;
   }

   while( ( p < 0 ) || ( p > menu_ppty_count ) )
   {
      printf( "Property: " );
      scanf( "%d", &p );
   }

   status = unicap_get_property( handle, &properties[p] );
   if( !SUCCESS( status ) )
   {
      fprintf( stderr, "Failed to inquire property '%s'\n", properties[p].identifier );
      exit( -1 );
   }

   printf( "Property '%s': Current = %s\n", properties[p].identifier,
	   properties[p].menu_item ); // (1)

   printf( "Menu Items: \n" );
   for( i = 0; i < properties[p].menu.menu_item_count; i++ ) // (2)
   {
      printf( "\t%d : %s\n", i, properties[p].menu.menu_items[i] ); // (3)
   }

   while( ( item < 0 ) || ( item >= properties[p].menu.menu_item_count ) )
   {
      printf( "Select menu item for property: " );
      scanf( "%d", &item );
   }

   strcpy( properties[p].menu_item, properties[p].menu.menu_items[item] ); // (4)

   if( !SUCCESS( unicap_set_property( handle, &properties[p] ) ) )
   {
      fprintf( stderr, "Failed to set property!\n" );
      exit( -1 );
   }
}


int
main (int argc, char **argv)
{
  unicap_handle_t handle;
  handle = open_device ();
  if (!handle)
  {
     fprintf( stderr, "Could not open video device\n" );
     return -1;
  }

  set_format(handle);

  set_menu_property( handle );

  unicap_close (handle);
  return 0;
}

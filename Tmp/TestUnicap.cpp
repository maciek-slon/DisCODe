#include <unicap.h>
#include <unicapgtk.h>
#include <string.h>

struct widgets
{
  GtkWidget *video_display;
  GtkWidget *format_selection;
  GtkWidget *property_dialog;
};

static void
format_change_cb (GtkWidget * ugtk, unicap_format_t * format,
                  struct widgets *widgets)
{
  unicapgtk_video_display_set_format (UNICAPGTK_VIDEO_DISPLAY
                                      (widgets->video_display), format);
}

static void
device_change_cb (UnicapgtkDeviceSelection * selection, gchar * device_id,
                  struct widgets *widgets)
{
  unicap_device_t device;
  unicap_handle_t handle;

  unicap_void_device (&device);
  strcpy (device.identifier, device_id);

  if (!SUCCESS (unicap_enumerate_devices (&device, &device, 0)) ||
      !SUCCESS (unicap_open (&handle, &device)))
    {
      // device is not available anymore
      g_printerr ("device '%s' not available!\n", device_id);
      return;
    }

  unicapgtk_video_format_selection_set_handle
    (UNICAPGTK_VIDEO_FORMAT_SELECTION (widgets->format_selection), handle);

  unicapgtk_video_display_stop (UNICAPGTK_VIDEO_DISPLAY
                                (widgets->video_display));
  unicapgtk_video_display_set_handle (UNICAPGTK_VIDEO_DISPLAY
                                      (widgets->video_display), handle);
  unicapgtk_video_display_start (UNICAPGTK_VIDEO_DISPLAY
                                 (widgets->video_display));

  unicapgtk_property_dialog_set_handle (UNICAPGTK_PROPERTY_DIALOG (widgets->property_dialog), handle);  // (2)
}


int
main (int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *device_selection;
  struct widgets widgets;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit),
                    NULL);

  vbox = gtk_vbox_new (FALSE, 10);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  hbox = gtk_hbox_new (FALSE, 10);
  gtk_box_pack_start_defaults (GTK_BOX (vbox), hbox);

  widgets.video_display = unicapgtk_video_display_new ();
  widgets.format_selection = unicapgtk_video_format_selection_new ();
  widgets.property_dialog = unicapgtk_property_dialog_new ();   // (1)

  device_selection = unicapgtk_device_selection_new (TRUE);
  gtk_box_pack_start_defaults (GTK_BOX (hbox), device_selection);
  gtk_box_pack_start_defaults (GTK_BOX (hbox), widgets.format_selection);
  unicapgtk_device_selection_rescan (UNICAPGTK_DEVICE_SELECTION
                                     (device_selection));
  g_signal_connect (G_OBJECT (device_selection),
                    "unicapgtk_device_selection_changed",
                    G_CALLBACK (device_change_cb), &widgets);
  gtk_combo_box_set_active (GTK_COMBO_BOX (device_selection), 0);

  g_signal_connect (G_OBJECT (widgets.format_selection),
                    "unicapgtk_video_format_changed",
                    G_CALLBACK (format_change_cb), &widgets);

  gtk_box_pack_start_defaults (GTK_BOX (vbox), widgets.video_display);

  gtk_widget_show_all (window);
  gtk_widget_show_all (widgets.property_dialog);
  gtk_main ();

  return 0;
}

/*
** Copyright (C) 2022 CNflysky. All rights reserved.
** Kernel DRM driver for W280BF036I LCD Panel in DSI interface.
** Driver IC: ST7701
*/

#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>
#include <drm/drm_print.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/regulator/consumer.h>
#include <video/mipi_display.h>

struct w280bf036i_panel_desc {
  const struct drm_display_mode *mode;
  unsigned int lanes;
  unsigned long flags;
  enum mipi_dsi_pixel_format format;
};

struct w280bf036i {
  struct drm_panel panel;
  struct mipi_dsi_device *dsi;
  const struct w280bf036i_panel_desc *desc;
  struct gpio_desc *reset;
};

static inline struct w280bf036i *panel_to_w280bf036i(struct drm_panel *panel) {
  return container_of(panel, struct w280bf036i, panel);
}

static inline int w280bf036i_dsi_write(struct w280bf036i *w280bf036i,
                                       const void *seq, size_t len) {
  return mipi_dsi_dcs_write_buffer(w280bf036i->dsi, seq, len);
}

#define w280bf036i_command(w280bf036i, seq...)          \
  {                                                     \
    const uint8_t d[] = {seq};                          \
    w280bf036i_dsi_write(w280bf036i, d, ARRAY_SIZE(d)); \
  }

static void w280bf036i_init_sequence(struct w280bf036i *w280bf036i) {
  // Command2 BK3 Selection: Enable the BK function of Command2
  w280bf036i_command(w280bf036i, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x13);
  // Unknown
  w280bf036i_command(w280bf036i, 0xEF, 0x08);
  // Command2 BK0 Selection: Disable the BK function of Command2
  w280bf036i_command(w280bf036i, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x10);
  // Display Line Setting
  w280bf036i_command(w280bf036i, 0xC0, 0x4f, 0x00);
  // Porch Control
  w280bf036i_command(w280bf036i, 0xC1, 0x10, 0x0c);
  // Inversion selection & Frame Rate Control
  w280bf036i_command(w280bf036i, 0xC2, 0x07, 0x14);
  // Unknown
  w280bf036i_command(w280bf036i, 0xCC, 0x10);
  // Positive Voltage Gamma Control
  w280bf036i_command(w280bf036i, 0xB0, 0x0a, 0x18, 0x1e, 0x12, 0x16, 0x0c, 0x0e,
                     0x0d, 0x0c, 0x29, 0x06, 0x14, 0x13, 0x29, 0x33, 0x1c);
  // Negative Voltage Gamma Control
  w280bf036i_command(w280bf036i, 0xB1, 0x0a, 0x19, 0x21, 0x0a, 0x0c, 0x00, 0x0c,
                     0x03, 0x03, 0x23, 0x01, 0x0e, 0x0c, 0x27, 0x2b, 0x1c);

  // Command2 BK1 Selection: Enable the BK function of Command2
  w280bf036i_command(w280bf036i, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x11);
  // Vop Amplitude setting
  w280bf036i_command(w280bf036i, 0xB0, 0x5d);
  // VCOM amplitude setting
  w280bf036i_command(w280bf036i, 0xB1, 0x61);
  // VGH Voltage setting
  w280bf036i_command(w280bf036i, 0xB2, 0x84);
  // TEST Command Setting
  w280bf036i_command(w280bf036i, 0xB3, 0x80);
  // VGL Voltage setting
  w280bf036i_command(w280bf036i, 0xB5, 0x4d);
  // Power Control 1
  w280bf036i_command(w280bf036i, 0xB7, 0x85);
  // Power Control 2
  w280bf036i_command(w280bf036i, 0xB8, 0x20);
  // Source pre_drive timing set1
  w280bf036i_command(w280bf036i, 0xC1, 0x78);
  // Source EQ2 Setting
  w280bf036i_command(w280bf036i, 0xC2, 0x78);
  // MIPI Setting 1
  w280bf036i_command(w280bf036i, 0xD0, 0x88);
  // Sunlight Readable Enhancement
  w280bf036i_command(w280bf036i, 0xE0, 0x00, 0x00, 0x02);
  // Noise Reduce Control
  w280bf036i_command(w280bf036i, 0xE1, 0x06, 0xa0, 0x08, 0xa0, 0x05, 0xa0, 0x07,
                     0xa0, 0x00, 0x44, 0x44);
  // Sharpness Control
  w280bf036i_command(w280bf036i, 0xE2, 0x20, 0x20, 0x44, 0x44, 0x96, 0xa0, 0x00,
                     0x00, 0x96, 0xa0, 0x00, 0x00);
  // Color Calibration Control
  w280bf036i_command(w280bf036i, 0xE3, 0x00, 0x00, 0x22, 0x22);
  // Skin Tone Preservation Control
  w280bf036i_command(w280bf036i, 0xE4, 0x44, 0x44);

  w280bf036i_command(w280bf036i, 0xE5, 0x0d, 0x91, 0xa0, 0xa0, 0x0f, 0x93, 0xa0,
                     0xa0, 0x09, 0x8d, 0xa0, 0xa0, 0x0b, 0x8f, 0xa0, 0xa0);
  w280bf036i_command(w280bf036i, 0xE6, 0x00, 0x00, 0x22, 0x22);
  w280bf036i_command(w280bf036i, 0xE7, 0x44, 0x44);
  w280bf036i_command(w280bf036i, 0xE8, 0x0c, 0x90, 0xa0, 0xa0, 0x0e, 0x92, 0xa0,
                     0xa0, 0x08, 0x8c, 0xa0, 0xa0, 0x0a, 0x8e, 0xa0, 0xa0);
  w280bf036i_command(w280bf036i, 0xE9, 0x36, 0x00);
  w280bf036i_command(w280bf036i, 0xEB, 0x00, 0x01, 0xe4, 0xe4, 0x44, 0x88,
                     0x40);
  w280bf036i_command(w280bf036i, 0xED, 0xff, 0x45, 0x67, 0xfa, 0x01, 0x2b, 0xcf,
                     0xff, 0xff, 0xfc, 0xb2, 0x10, 0xaf, 0x76, 0x54, 0xff);
  w280bf036i_command(w280bf036i, 0xEF, 0x10, 0x0d, 0x04, 0x08, 0x3f, 0x1f);
  /* disable Command2 */
  // w280bf036i_command(w280bf036i, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x00);
}

static int w280bf036i_prepare(struct drm_panel *panel) {
  struct w280bf036i *w280bf036i = panel_to_w280bf036i(panel);
  gpiod_set_value(w280bf036i->reset, 0);

  msleep(50);
  gpiod_set_value(w280bf036i->reset, 1);
  msleep(150);
  mipi_dsi_dcs_soft_reset(w280bf036i->dsi);

  msleep(5);

  w280bf036i_init_sequence(w280bf036i);

  mipi_dsi_dcs_set_tear_on(w280bf036i->dsi, MIPI_DSI_DCS_TEAR_MODE_VBLANK);
  mipi_dsi_dcs_exit_sleep_mode(w280bf036i->dsi);
  return 0;
}

static int w280bf036i_enable(struct drm_panel *panel) {
  return mipi_dsi_dcs_set_display_on(panel_to_w280bf036i(panel)->dsi);
}

static int w280bf036i_disable(struct drm_panel *panel) {
  return mipi_dsi_dcs_set_display_off(panel_to_w280bf036i(panel)->dsi);
}

static int w280bf036i_unprepare(struct drm_panel *panel) {
  struct w280bf036i *w280bf036i = panel_to_w280bf036i(panel);

  mipi_dsi_dcs_enter_sleep_mode(w280bf036i->dsi);

  gpiod_set_value(w280bf036i->reset, 0);

  return 0;
}

static int w280bf036i_get_modes(struct drm_panel *panel,
                                struct drm_connector *connector) {
  struct w280bf036i *w280bf036i = panel_to_w280bf036i(panel);
  const struct drm_display_mode *desc_mode = w280bf036i->desc->mode;
  struct drm_display_mode *mode;

  mode = drm_mode_duplicate(connector->dev, desc_mode);
  if (!mode) {
    dev_err(&w280bf036i->dsi->dev, "failed to add mode %ux%u@%u\n",
            desc_mode->hdisplay, desc_mode->vdisplay,
            drm_mode_vrefresh(desc_mode));
    return -ENOMEM;
  }

  drm_mode_set_name(mode);
  drm_mode_probed_add(connector, mode);

  connector->display_info.width_mm = desc_mode->width_mm;
  connector->display_info.height_mm = desc_mode->height_mm;

  return 1;
}

static const struct drm_panel_funcs w280bf036i_funcs = {
    .disable = w280bf036i_disable,
    .unprepare = w280bf036i_unprepare,
    .prepare = w280bf036i_prepare,
    .enable = w280bf036i_enable,
    .get_modes = w280bf036i_get_modes,
};

static const struct drm_display_mode w280bf036i_mode = {
    .clock = 25000,

    .hdisplay = 480,
    .hsync_start = 480 + /* HFP */ 10,
    .hsync_end = 480 + 10 + /* HSync */ 4,
    .htotal = 480 + 10 + 4 + /* HBP */ 20,

    .vdisplay = 640,
    .vsync_start = 640 + /* VFP */ 8,
    .vsync_end = 640 + 8 + /* VSync */ 4,
    .vtotal = 640 + 8 + 4 + /* VBP */ 14,

    .width_mm = 43,
    .height_mm = 57,

    .type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
};

static const struct w280bf036i_panel_desc w280bf036i_desc = {
    .mode = &w280bf036i_mode,
    .lanes = 1,
    .flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_BURST,
    .format = MIPI_DSI_FMT_RGB888,
};

static int w280bf036i_dsi_probe(struct mipi_dsi_device *dsi) {
  struct w280bf036i *w280bf036i =
      devm_kzalloc(&dsi->dev, sizeof(*w280bf036i), GFP_KERNEL);
  if (!w280bf036i) return -ENOMEM;

  const struct w280bf036i_panel_desc *desc =
      of_device_get_match_data(&dsi->dev);
  dsi->mode_flags = desc->flags;
  dsi->format = desc->format;
  dsi->lanes = desc->lanes;

  w280bf036i->reset = devm_gpiod_get(&dsi->dev, "reset", GPIOD_OUT_LOW);
  if (IS_ERR(w280bf036i->reset)) {
    dev_err(&dsi->dev, "Couldn't get our reset GPIO\n");
    return PTR_ERR(w280bf036i->reset);
  }

  drm_panel_init(&w280bf036i->panel, &dsi->dev, &w280bf036i_funcs,
                 DRM_MODE_CONNECTOR_DSI);

  int ret = drm_panel_of_backlight(&w280bf036i->panel);
  if (ret) return ret;

  drm_panel_add(&w280bf036i->panel);

  mipi_dsi_set_drvdata(dsi, w280bf036i);
  w280bf036i->dsi = dsi;
  w280bf036i->desc = desc;

  return mipi_dsi_attach(dsi);
}

static int w280bf036i_dsi_remove(struct mipi_dsi_device *dsi) {
  struct w280bf036i *w280bf036i = mipi_dsi_get_drvdata(dsi);

  mipi_dsi_detach(dsi);
  drm_panel_remove(&w280bf036i->panel);

  return 0;
}

static const struct of_device_id w280bf036i_of_match[] = {
    {.compatible = "wlk,w280bf036i", .data = &w280bf036i_desc}, {}};
MODULE_DEVICE_TABLE(of, w280bf036i_of_match);

static struct mipi_dsi_driver w280bf036i_dsi_driver = {
    .probe = w280bf036i_dsi_probe,
    .remove = w280bf036i_dsi_remove,
    .driver =
        {
            .name = "w280bf036i",
            .of_match_table = w280bf036i_of_match,
        },
};
module_mipi_dsi_driver(w280bf036i_dsi_driver);

MODULE_AUTHOR("CNflysky@qq.com");
MODULE_DESCRIPTION("WLK w280bf036i LCD Panel Driver");
MODULE_LICENSE("GPL");

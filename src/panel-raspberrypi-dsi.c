/*
** Copyright (C) 2022 CNflysky. All rights reserved.
** Kernel DRM driver for Multiple Panels in DSI interface.
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

struct dsi_panel_desc {
  const struct drm_display_mode *mode;
  unsigned int lanes;
  unsigned long flags;
  enum mipi_dsi_pixel_format format;
  void (*init_sequence)(struct mipi_dsi_device *dsi);
};

struct dsi_panel {
  struct drm_panel panel;
  struct mipi_dsi_device *dsi;
  const struct dsi_panel_desc *desc;
  struct gpio_desc *reset;
};

static inline struct dsi_panel *to_dsi_panel(struct drm_panel *panel) {
  return container_of(panel, struct dsi_panel, panel);
}

static inline int panel_dsi_write(struct mipi_dsi_device *dsi, const void *seq,
                                  size_t len) {
  return mipi_dsi_dcs_write_buffer(dsi, seq, len);
}

#define panel_command(dsi, seq...)          \
  {                                         \
    const uint8_t d[] = {seq};              \
    panel_dsi_write(dsi, d, ARRAY_SIZE(d)); \
  }

static void w280bf036i_init_sequence(struct mipi_dsi_device *dsi) {
  // Command2 BK3 Selection: Enable the BK function of Command2
  panel_command(dsi, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x13);
  // Unknown
  panel_command(dsi, 0xEF, 0x08);
  // Command2 BK0 Selection: Disable the BK function of Command2
  panel_command(dsi, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x10);
  // Display Line Setting
  panel_command(dsi, 0xC0, 0x4f, 0x00);
  // Porch Control
  panel_command(dsi, 0xC1, 0x10, 0x0c);
  // Inversion selection & Frame Rate Control
  panel_command(dsi, 0xC2, 0x07, 0x14);
  // Unknown
  panel_command(dsi, 0xCC, 0x10);
  // Positive Voltage Gamma Control
  panel_command(dsi, 0xB0, 0x0a, 0x18, 0x1e, 0x12, 0x16, 0x0c, 0x0e, 0x0d, 0x0c,
                0x29, 0x06, 0x14, 0x13, 0x29, 0x33, 0x1c);
  // Negative Voltage Gamma Control
  panel_command(dsi, 0xB1, 0x0a, 0x19, 0x21, 0x0a, 0x0c, 0x00, 0x0c, 0x03, 0x03,
                0x23, 0x01, 0x0e, 0x0c, 0x27, 0x2b, 0x1c);

  // Command2 BK1 Selection: Enable the BK function of Command2
  panel_command(dsi, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x11);
  // Vop Amplitude setting
  panel_command(dsi, 0xB0, 0x5d);
  // VCOM amplitude setting
  panel_command(dsi, 0xB1, 0x61);
  // VGH Voltage setting
  panel_command(dsi, 0xB2, 0x84);
  // TEST Command Setting
  panel_command(dsi, 0xB3, 0x80);
  // VGL Voltage setting
  panel_command(dsi, 0xB5, 0x4d);
  // Power Control 1
  panel_command(dsi, 0xB7, 0x85);
  // Power Control 2
  panel_command(dsi, 0xB8, 0x20);
  // Source pre_drive timing set1
  panel_command(dsi, 0xC1, 0x78);
  // Source EQ2 Setting
  panel_command(dsi, 0xC2, 0x78);
  // MIPI Setting 1
  panel_command(dsi, 0xD0, 0x88);
  // GIP Code
  panel_command(dsi, 0xE0, 0x00, 0x00, 0x02);
  panel_command(dsi, 0xE1, 0x06, 0xa0, 0x08, 0xa0, 0x05, 0xa0, 0x07, 0xa0, 0x00,
                0x44, 0x44);
  panel_command(dsi, 0xE2, 0x20, 0x20, 0x44, 0x44, 0x96, 0xa0, 0x00, 0x00, 0x96,
                0xa0, 0x00, 0x00);
  panel_command(dsi, 0xE3, 0x00, 0x00, 0x22, 0x22);
  panel_command(dsi, 0xE4, 0x44, 0x44);
  panel_command(dsi, 0xE5, 0x0d, 0x91, 0xa0, 0xa0, 0x0f, 0x93, 0xa0, 0xa0, 0x09,
                0x8d, 0xa0, 0xa0, 0x0b, 0x8f, 0xa0, 0xa0);
  panel_command(dsi, 0xE6, 0x00, 0x00, 0x22, 0x22);
  panel_command(dsi, 0xE7, 0x44, 0x44);
  panel_command(dsi, 0xE8, 0x0c, 0x90, 0xa0, 0xa0, 0x0e, 0x92, 0xa0, 0xa0, 0x08,
                0x8c, 0xa0, 0xa0, 0x0a, 0x8e, 0xa0, 0xa0);
  panel_command(dsi, 0xE9, 0x36, 0x00);
  panel_command(dsi, 0xEB, 0x00, 0x01, 0xe4, 0xe4, 0x44, 0x88, 0x40);
  panel_command(dsi, 0xED, 0xff, 0x45, 0x67, 0xfa, 0x01, 0x2b, 0xcf, 0xff, 0xff,
                0xfc, 0xb2, 0x10, 0xaf, 0x76, 0x54, 0xff);
  panel_command(dsi, 0xEF, 0x10, 0x0d, 0x04, 0x08, 0x3f, 0x1f);
  // disable Command2
  panel_command(dsi, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x00);
}

static void w500ie020_init_sequence(struct mipi_dsi_device *dsi) {
  panel_command(dsi, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x10);
  // Display Line Setting
  panel_command(dsi, 0xC0, 0xe9, 0x03);
  // Porch Control
  panel_command(dsi, 0xC1, 0x12, 0x02);
  // Inversion selection & Frame Rate Control
  panel_command(dsi, 0xC2, 0x37, 0x08);
  // Unknown
  panel_command(dsi, 0xCC, 0x10);
  // Positive Voltage Gamma Control
  panel_command(dsi, 0xB0, 0x00, 0x0E, 0x15, 0x0F, 0x11, 0x08, 0x08, 0x08, 0x08,
                0x23, 0x04, 0x13, 0x12, 0x2B, 0x34, 0x1F);
  // Negative Voltage Gamma Control
  panel_command(dsi, 0xB1, 0x00, 0x0E, 0x95, 0x0F, 0x13, 0x07, 0x09, 0x08, 0x08,
                0x22, 0x04, 0x10, 0x0E, 0x2C, 0x34, 0x1F);

  // Command2 BK1 Selection: Enable the BK function of Command2
  panel_command(dsi, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x11);
  // Vop Amplitude setting
  panel_command(dsi, 0xB0, 0x4d);
  // VCOM amplitude setting
  panel_command(dsi, 0xB1, 0x13);
  // VGH Voltage setting
  panel_command(dsi, 0xB2, 0x07);
  // TEST Command Setting
  panel_command(dsi, 0xB3, 0x80);
  // VGL Voltage setting
  panel_command(dsi, 0xB5, 0x47);
  // Power Control 1
  panel_command(dsi, 0xB7, 0x85);
  // Power Control 2
  panel_command(dsi, 0xB8, 0x20);
  // Unknown
  panel_command(dsi, 0xB9, 0x10);
  // Source pre_drive timing set1
  panel_command(dsi, 0xC1, 0x78);
  // Source EQ2 Setting
  panel_command(dsi, 0xC2, 0x78);
  // MIPI Setting 1
  panel_command(dsi, 0xD0, 0x88);
  // GIP Code
  panel_command(dsi, 0xE0, 0x00, 0x00, 0x02);
  panel_command(dsi, 0x0B, 0x00, 0x0D, 0x00, 0x0C, 0x00, 0x0E, 0x00, 0x00, 0x44,
                0x44);
  panel_command(dsi, 0xE2, 0x33, 0x33, 0x44, 0x44, 0x64, 0x00, 0x66, 0x00, 0x65,
                0x00, 0x67, 0x00, 0x00);
  panel_command(dsi, 0xE3, 0x00, 0x00, 0x33, 0x33);
  panel_command(dsi, 0xE4, 0x44, 0x44);

  panel_command(dsi, 0xE5, 0x0C, 0x78, 0x3C, 0xA0, 0x0E, 0x78, 0x3C, 0xA0, 0x10,
                0x78, 0x3C, 0xA0, 0x12, 0x78, 0x3C, 0xA0);
  panel_command(dsi, 0xE6, 0x00, 0x00, 0x33, 0x33);
  panel_command(dsi, 0xE7, 0x44, 0x44);
  panel_command(dsi, 0xE8, 0x0d, 0x78, 0x3C, 0xA0, 0x0F, 0x78, 0x3C, 0xA0, 0x11,
                0x78, 0x3C, 0xA0, 0x13, 0x78, 0x3C, 0xA0);
  panel_command(dsi, 0xEB, 0x02, 0x02, 0x39, 0x39, 0xEE, 0x44, 0x00);
  panel_command(dsi, 0xEC, 0x00, 0x00);
  panel_command(dsi, 0xED, 0xFF, 0xF1, 0x04, 0x56, 0x72, 0x3F, 0xFF, 0xFF, 0xFF,
                0xFF, 0xF3, 0x27, 0x65, 0x40, 0x1F, 0xFF);
  panel_command(dsi, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x00);
}

static int dsi_panel_prepare(struct drm_panel *panel) {
  struct dsi_panel *dsi_panel = to_dsi_panel(panel);
  gpiod_set_value_cansleep(dsi_panel->reset, 0);
  msleep(30);
  gpiod_set_value_cansleep(dsi_panel->reset, 1);
  msleep(150);

  mipi_dsi_dcs_soft_reset(dsi_panel->dsi);
  msleep(30);

  dsi_panel->desc->init_sequence(dsi_panel->dsi);

  mipi_dsi_dcs_set_tear_on(dsi_panel->dsi, MIPI_DSI_DCS_TEAR_MODE_VBLANK);
  mipi_dsi_dcs_exit_sleep_mode(dsi_panel->dsi);
  msleep(120);
  return 0;
}

static int dsi_panel_enable(struct drm_panel *panel) {
  return mipi_dsi_dcs_set_display_on(to_dsi_panel(panel)->dsi);
}

static int dsi_panel_disable(struct drm_panel *panel) {
  return mipi_dsi_dcs_set_display_off(to_dsi_panel(panel)->dsi);
}

static int dsi_panel_unprepare(struct drm_panel *panel) {
  struct dsi_panel *dsi_panel = to_dsi_panel(panel);

  mipi_dsi_dcs_enter_sleep_mode(dsi_panel->dsi);

  gpiod_set_value_cansleep(dsi_panel->reset, 0);

  return 0;
}

static int dsi_panel_get_modes(struct drm_panel *panel,
                               struct drm_connector *connector) {
  struct dsi_panel *dsi_panel = to_dsi_panel(panel);
  const struct drm_display_mode *desc_mode = dsi_panel->desc->mode;
  struct drm_display_mode *mode;

  mode = drm_mode_duplicate(connector->dev, desc_mode);
  if (!mode) {
    dev_err(&dsi_panel->dsi->dev, "failed to add mode %ux%u@%u\n",
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

static const struct drm_panel_funcs dsi_panel_funcs = {
    .disable = dsi_panel_disable,
    .unprepare = dsi_panel_unprepare,
    .prepare = dsi_panel_prepare,
    .enable = dsi_panel_enable,
    .get_modes = dsi_panel_get_modes,
};

static const struct drm_display_mode w280bf036i_mode = {
    .clock = 22572,

    .hdisplay = 480,
    .hsync_start = 480 + /* HFP */ 30,
    .hsync_end = 480 + 30 + /* HSync */ 10,
    .htotal = 480 + 30 + 10 + /* HBP */ 30,

    .vdisplay = 640,
    .vsync_start = 640 + /* VFP */ 20,
    .vsync_end = 640 + 20 + /* VSync */ 4,
    .vtotal = 640 + 20 + 4 + /* VBP */ 20,

    .width_mm = 43,
    .height_mm = 57,

    .type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
};

static const struct drm_display_mode w500ie020_mode = {
    .clock = 20500,

    .hdisplay = 480,
    .hsync_start = 480 + /* HFP */ 10,
    .hsync_end = 480 + 10 + /* HSync */ 4,
    .htotal = 480 + 10 + 4 + /* HBP */ 20,

    .vdisplay = 854,
    .vsync_start = 854 + /* VFP */ 8,
    .vsync_end = 854 + 8 + /* VSync */ 4,
    .vtotal = 854 + 8 + 4 + /* VBP */ 14,

    .width_mm = 43,
    .height_mm = 58,

    .type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED,
};

static const struct dsi_panel_desc w280bf036i_desc = {
    .mode = &w280bf036i_mode,
    .lanes = 1,
    .flags =
        MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_BURST | MIPI_DSI_MODE_LPM,
    .format = MIPI_DSI_FMT_RGB888,
    .init_sequence = w280bf036i_init_sequence};

static const struct dsi_panel_desc w500ie020_desc = {
    .mode = &w500ie020_mode,
    .lanes = 2,
    .flags =
        MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_BURST | MIPI_DSI_MODE_LPM,
    .format = MIPI_DSI_FMT_RGB888,
    .init_sequence = w500ie020_init_sequence};

static int dsi_panel_probe(struct mipi_dsi_device *dsi) {
  struct dsi_panel *dsi_panel =
      devm_kzalloc(&dsi->dev, sizeof(*dsi_panel), GFP_KERNEL);
  if (!dsi_panel) return -ENOMEM;

  const struct dsi_panel_desc *desc = of_device_get_match_data(&dsi->dev);
  dsi->mode_flags = desc->flags;
  dsi->format = desc->format;
  dsi->lanes = desc->lanes;

  dsi_panel->panel.prepare_upstream_first = true;
  dsi_panel->reset = devm_gpiod_get(&dsi->dev, "reset", GPIOD_OUT_LOW);
  if (IS_ERR(dsi_panel->reset)) {
    dev_err(&dsi->dev, "Couldn't get our reset GPIO\n");
    return PTR_ERR(dsi_panel->reset);
  } else
    drm_panel_init(&dsi_panel->panel, &dsi->dev, &dsi_panel_funcs,
                   DRM_MODE_CONNECTOR_DSI);

  int ret = drm_panel_of_backlight(&dsi_panel->panel);
  if (ret) return ret;

  drm_panel_add(&dsi_panel->panel);

  mipi_dsi_set_drvdata(dsi, dsi_panel);
  dsi_panel->dsi = dsi;
  dsi_panel->desc = desc;
  if ((ret = mipi_dsi_attach(dsi))) drm_panel_remove(&dsi_panel->panel);
  return ret;
}

static void dsi_panel_remove(struct mipi_dsi_device *dsi) {
  struct dsi_panel *dsi_panel = mipi_dsi_get_drvdata(dsi);

  mipi_dsi_detach(dsi);
  drm_panel_remove(&dsi_panel->panel);
}

static const struct of_device_id dsi_panel_of_match[] = {
    {.compatible = "wlk,w280bf036i", .data = &w280bf036i_desc},
    {.compatible = "wlk,w500ie020", .data = &w500ie020_desc},
    {}};

MODULE_DEVICE_TABLE(of, dsi_panel_of_match);

static struct mipi_dsi_driver dsi_panel_driver = {
    .probe = dsi_panel_probe,
    .remove = dsi_panel_remove,
    .driver =
        {
            .name = "dsi_panel_driver",
            .of_match_table = dsi_panel_of_match,
        },
};
module_mipi_dsi_driver(dsi_panel_driver);

#ifdef HAS_PCA9536
MODULE_SOFTDEP("pre: gpio_pca953x");
#endif

MODULE_AUTHOR("CNflysky@qq.com");
MODULE_DESCRIPTION("Multiple Raspberry Pi DSI Panels Driver");
MODULE_LICENSE("GPL");

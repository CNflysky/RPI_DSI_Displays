/*
** Copyright (C) 2024 CNflysky. All rights reserved.
*/

#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <video/mipi_display.h>
#include <linux/of.h>

struct rpi_dsi_display_desc {
	const struct drm_display_mode *mode;
	unsigned int lanes;
	unsigned long flags;
	enum mipi_dsi_pixel_format format;
	void (*init_sequence)(struct mipi_dsi_device *dsi);
};

struct rpi_dsi_display {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
	const struct rpi_dsi_display_desc *desc;
	struct gpio_desc *reset;
	enum drm_panel_orientation orientation;
};

static inline struct rpi_dsi_display *
to_rpi_dsi_display(struct drm_panel *panel)
{
	return container_of(panel, struct rpi_dsi_display, panel);
}

static inline int rpi_dsi_display_write(struct mipi_dsi_device *dsi,
					const void *seq, size_t len)
{
	return mipi_dsi_dcs_write_buffer(dsi, seq, len);
}

#define rpi_dsi_display_cmd(dsi, seq...)                            \
	{                                                           \
		const uint8_t data[] = { seq };                     \
		rpi_dsi_display_write(dsi, data, ARRAY_SIZE(data)); \
	}

static inline void w280bf036i_init_sequence(struct mipi_dsi_device *dsi)
{
	// Command2 BK3 Selection: Enable the BK function of Command2
	rpi_dsi_display_cmd(dsi, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x13);
	// Unknown
	rpi_dsi_display_cmd(dsi, 0xEF, 0x08);
	// Command2 BK0 Selection: Disable the BK function of Command2
	rpi_dsi_display_cmd(dsi, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x10);
	// Display Line Setting
	rpi_dsi_display_cmd(dsi, 0xC0, 0x4f, 0x00);
	// Porch Control
	rpi_dsi_display_cmd(dsi, 0xC1, 0x10, 0x0c);
	// Inversion selection & Frame Rate Control
	rpi_dsi_display_cmd(dsi, 0xC2, 0x07, 0x14);
	// Unknown
	rpi_dsi_display_cmd(dsi, 0xCC, 0x10);
	// Positive Voltage Gamma Control
	rpi_dsi_display_cmd(dsi, 0xB0, 0x0a, 0x18, 0x1e, 0x12, 0x16, 0x0c, 0x0e,
			    0x0d, 0x0c, 0x29, 0x06, 0x14, 0x13, 0x29, 0x33,
			    0x1c);
	// Negative Voltage Gamma Control
	rpi_dsi_display_cmd(dsi, 0xB1, 0x0a, 0x19, 0x21, 0x0a, 0x0c, 0x00, 0x0c,
			    0x03, 0x03, 0x23, 0x01, 0x0e, 0x0c, 0x27, 0x2b,
			    0x1c);

	// Command2 BK1 Selection: Enable the BK function of Command2
	rpi_dsi_display_cmd(dsi, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x11);
	// Vop Amplitude setting
	rpi_dsi_display_cmd(dsi, 0xB0, 0x5d);
	// VCOM amplitude setting
	rpi_dsi_display_cmd(dsi, 0xB1, 0x61);
	// VGH Voltage setting
	rpi_dsi_display_cmd(dsi, 0xB2, 0x84);
	// TEST Command Setting
	rpi_dsi_display_cmd(dsi, 0xB3, 0x80);
	// VGL Voltage setting
	rpi_dsi_display_cmd(dsi, 0xB5, 0x4d);
	// Power Control 1
	rpi_dsi_display_cmd(dsi, 0xB7, 0x85);
	// Power Control 2
	rpi_dsi_display_cmd(dsi, 0xB8, 0x20);
	// Source pre_drive timing set1
	rpi_dsi_display_cmd(dsi, 0xC1, 0x78);
	// Source EQ2 Setting
	rpi_dsi_display_cmd(dsi, 0xC2, 0x78);
	// MIPI Setting 1
	rpi_dsi_display_cmd(dsi, 0xD0, 0x88);
	// GIP Code
	rpi_dsi_display_cmd(dsi, 0xE0, 0x00, 0x00, 0x02);
	rpi_dsi_display_cmd(dsi, 0xE1, 0x06, 0xa0, 0x08, 0xa0, 0x05, 0xa0, 0x07,
			    0xa0, 0x00, 0x44, 0x44);
	rpi_dsi_display_cmd(dsi, 0xE2, 0x20, 0x20, 0x44, 0x44, 0x96, 0xa0, 0x00,
			    0x00, 0x96, 0xa0, 0x00, 0x00);
	rpi_dsi_display_cmd(dsi, 0xE3, 0x00, 0x00, 0x22, 0x22);
	rpi_dsi_display_cmd(dsi, 0xE4, 0x44, 0x44);
	rpi_dsi_display_cmd(dsi, 0xE5, 0x0d, 0x91, 0xa0, 0xa0, 0x0f, 0x93, 0xa0,
			    0xa0, 0x09, 0x8d, 0xa0, 0xa0, 0x0b, 0x8f, 0xa0,
			    0xa0);
	rpi_dsi_display_cmd(dsi, 0xE6, 0x00, 0x00, 0x22, 0x22);
	rpi_dsi_display_cmd(dsi, 0xE7, 0x44, 0x44);
	rpi_dsi_display_cmd(dsi, 0xE8, 0x0c, 0x90, 0xa0, 0xa0, 0x0e, 0x92, 0xa0,
			    0xa0, 0x08, 0x8c, 0xa0, 0xa0, 0x0a, 0x8e, 0xa0,
			    0xa0);
	rpi_dsi_display_cmd(dsi, 0xE9, 0x36, 0x00);
	rpi_dsi_display_cmd(dsi, 0xEB, 0x00, 0x01, 0xe4, 0xe4, 0x44, 0x88,
			    0x40);
	rpi_dsi_display_cmd(dsi, 0xED, 0xff, 0x45, 0x67, 0xfa, 0x01, 0x2b, 0xcf,
			    0xff, 0xff, 0xfc, 0xb2, 0x10, 0xaf, 0x76, 0x54,
			    0xff);
	rpi_dsi_display_cmd(dsi, 0xEF, 0x10, 0x0d, 0x04, 0x08, 0x3f, 0x1f);
	// disable Command2
	rpi_dsi_display_cmd(dsi, 0xFF, 0x77, 0x01, 0x00, 0x00, 0x00);
}

static int rpi_dsi_display_prepare(struct drm_panel *panel)
{
	struct rpi_dsi_display *rpi_dsi_display = to_rpi_dsi_display(panel);
	gpiod_set_value_cansleep(rpi_dsi_display->reset, 0);
	usleep_range(30 * 1000, 50 * 1000);
	gpiod_set_value_cansleep(rpi_dsi_display->reset, 1);
	usleep_range(150 * 1000, 200 * 1000);

	mipi_dsi_dcs_soft_reset(rpi_dsi_display->dsi);
	usleep_range(30 * 1000, 50 * 1000);

	rpi_dsi_display->desc->init_sequence(rpi_dsi_display->dsi);

	mipi_dsi_dcs_set_tear_on(rpi_dsi_display->dsi,
				 MIPI_DSI_DCS_TEAR_MODE_VBLANK);
	mipi_dsi_dcs_exit_sleep_mode(rpi_dsi_display->dsi);
	msleep(120);
	return 0;
}

inline static int rpi_dsi_display_enable(struct drm_panel *panel)
{
	return mipi_dsi_dcs_set_display_on(to_rpi_dsi_display(panel)->dsi);
}

inline static int rpi_dsi_display_disable(struct drm_panel *panel)
{
	return mipi_dsi_dcs_set_display_off(to_rpi_dsi_display(panel)->dsi);
}

static int rpi_dsi_display_unprepare(struct drm_panel *panel)
{
	struct rpi_dsi_display *rpi_dsi_display = to_rpi_dsi_display(panel);

	mipi_dsi_dcs_enter_sleep_mode(rpi_dsi_display->dsi);

	gpiod_set_value_cansleep(rpi_dsi_display->reset, 0);

	return 0;
}

static int rpi_dsi_display_get_modes(struct drm_panel *panel,
				     struct drm_connector *connector)
{
	struct rpi_dsi_display *rpi_dsi_display = to_rpi_dsi_display(panel);
	const struct drm_display_mode *desc_mode = rpi_dsi_display->desc->mode;
	struct drm_display_mode *mode;

	mode = drm_mode_duplicate(connector->dev, desc_mode);
	if (!mode) {
		dev_err(&rpi_dsi_display->dsi->dev,
			"failed to add mode %ux%u@%u\n", desc_mode->hdisplay,
			desc_mode->vdisplay, drm_mode_vrefresh(desc_mode));
		return -ENOMEM;
	}

	drm_mode_set_name(mode);
	drm_mode_probed_add(connector, mode);

	connector->display_info.width_mm = desc_mode->width_mm;
	connector->display_info.height_mm = desc_mode->height_mm;

	drm_connector_set_orientation_from_panel(connector, panel);
	return 1;
}

static inline enum drm_panel_orientation
rpi_dsi_display_get_orientation(struct drm_panel *panel)
{
	struct rpi_dsi_display *rpi_dsi_display = to_rpi_dsi_display(panel);
	return rpi_dsi_display->orientation;
}

static const struct drm_panel_funcs rpi_dsi_display_funcs = {
	.disable = rpi_dsi_display_disable,
	.unprepare = rpi_dsi_display_unprepare,
	.prepare = rpi_dsi_display_prepare,
	.enable = rpi_dsi_display_enable,
	.get_modes = rpi_dsi_display_get_modes,
	.get_orientation = rpi_dsi_display_get_orientation,
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

static const struct rpi_dsi_display_desc w280bf036i_desc = {
	.mode = &w280bf036i_mode,
	.lanes = 1,
	.flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_BURST |
		 MIPI_DSI_MODE_LPM,
	.format = MIPI_DSI_FMT_RGB888,
	.init_sequence = w280bf036i_init_sequence
};

static int rpi_dsi_display_probe(struct mipi_dsi_device *dsi)
{
	struct rpi_dsi_display *rpi_dsi_display =
		devm_kzalloc(&dsi->dev, sizeof(*rpi_dsi_display), GFP_KERNEL);
	if (!rpi_dsi_display)
		return -ENOMEM;

	const struct rpi_dsi_display_desc *desc =
		of_device_get_match_data(&dsi->dev);
	dsi->mode_flags = desc->flags;
	dsi->format = desc->format;
	dsi->lanes = desc->lanes;

	rpi_dsi_display->panel.prepare_prev_first = true;
	rpi_dsi_display->reset =
		devm_gpiod_get(&dsi->dev, "reset", GPIOD_OUT_LOW);

	if (IS_ERR(rpi_dsi_display->reset)) {
		dev_err(&dsi->dev, "Failed to get reset GPIO\n");
		return PTR_ERR(rpi_dsi_display->reset);
	}

	int ret = of_drm_get_panel_orientation(dsi->dev.of_node,
					       &rpi_dsi_display->orientation);
	if (ret < 0) {
		dev_warn(&dsi->dev,
			 "Failed to get orientation, default to normal");
		rpi_dsi_display->orientation =
			DRM_MODE_PANEL_ORIENTATION_NORMAL;
	}

	drm_panel_init(&rpi_dsi_display->panel, &dsi->dev,
		       &rpi_dsi_display_funcs, DRM_MODE_CONNECTOR_DSI);

	ret = drm_panel_of_backlight(&rpi_dsi_display->panel);
	if (IS_ERR(&ret))
		return ret;

	drm_panel_add(&rpi_dsi_display->panel);

	mipi_dsi_set_drvdata(dsi, rpi_dsi_display);
	rpi_dsi_display->dsi = dsi;
	rpi_dsi_display->desc = desc;
	if ((ret = mipi_dsi_attach(dsi)))
		drm_panel_remove(&rpi_dsi_display->panel);
	return ret;
}

static void rpi_dsi_display_remove(struct mipi_dsi_device *dsi)
{
	struct rpi_dsi_display *rpi_dsi_display = mipi_dsi_get_drvdata(dsi);

	mipi_dsi_detach(dsi);
	drm_panel_remove(&rpi_dsi_display->panel);
}

static const struct of_device_id rpi_dsi_display_of_match[] = {
	{ .compatible = "wlk,w280bf036i", .data = &w280bf036i_desc },
	{}
};

MODULE_DEVICE_TABLE(of, rpi_dsi_display_of_match);

static struct mipi_dsi_driver rpi_dsi_display = {
    .probe = rpi_dsi_display_probe,
    .remove = rpi_dsi_display_remove,
    .driver =
        {
            .name = "rpi_dsi_display_driver",
            .of_match_table = rpi_dsi_display_of_match,
        },
};

module_mipi_dsi_driver(rpi_dsi_display);
MODULE_AUTHOR("cnflysky@qq.com");
MODULE_DESCRIPTION("RPI DSI Display driver");
MODULE_LICENSE("GPL");

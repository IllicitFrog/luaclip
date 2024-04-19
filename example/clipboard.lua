local luaclip = require("libluaclip")
local awful = require("awful")
local wibox = require("wibox")
local utils = require("main.utils")
local beautiful = require("beautiful")
local rubato = require("rubato")

local M = {}

M.lclip = luaclip()

M.clips = {}

M.prompt = wibox.widget({
	font = beautiful.font_medium,
	widget = wibox.widget.textbox,
	text = "Clipboard",
})

M.icon = wibox.widget({
	image = beautiful.config_path .. "assets/clipboard.png",
	forced_height = beautiful.dpi(50),
	forced_width = beautiful.dpi(50),
	widget = wibox.widget.imagebox,
})

M.input = wibox.widget({
	widget = wibox.widget.textbox,
})

M.popup = awful.popup({
	ontop = true,
	visible = false,
	widget = wibox.container.background,
	placement = function(c)
		awful.placement.centered(c, { margins = { top = beautiful.dpi(240) } })
	end,
	screen = vars.centerM,
	bg = beautiful.gradient,
	opacity = 0,
	shape = utils.rrect(),
})

M.create = function()
	M.lclip:daemon()

	for i = 1, 5, 1 do
		M.clips[i] = wibox.widget({
			wibox.widget({
				wibox.widget({
					valign = "top",
					halign = "left",
					widget = wibox.widget.textbox,
				}),
				fg = beautiful.yellow,
				widget = wibox.container.background,
			}),
			{
				wibox.widget({
					valign = "top",
					halign = "left",
					widget = wibox.widget.textbox,
				}),
				widget = wibox.container.scroll.vertical,
				forced_height = beautiful.dpi(50),
			},
			spacing = beautiful.dpi(10),
			layout = wibox.layout.fixed.horizontal,
		})
	end

	M.popup:setup({
		{
			{
				M.icon,
				utils.add_bg(M.prompt, nil, beautiful.yellow),
				spacing = beautiful.dpi(10),
				layout = wibox.layout.fixed.horizontal,
			},
			{
				wibox.widget.separator({
					orientation = "horizontal",
					color = beautiful.magenta,
					thickness = beautiful.dpi(1),
					forced_height = beautiful.dpi(5),
					forced_width = beautiful.dpi(220),
				}),
				halign = "left",
				widget = wibox.container.place,
			},
			{
				{
					{
						{
							wibox.widget({
								wibox.widget({
									text = "  ",
									font = "SFMonoNerdFontMono Bold 14",
									widget = wibox.widget.textbox,
								}),
								fg = beautiful.yellow,
								widget = wibox.widget.background,
							}),
							M.input,
							spacing = beautiful.dpi(10),
							layout = wibox.layout.fixed.horizontal,
						},
						shape = utils.rrect(),
						bg = beautiful.bg0,
						forced_width = beautiful.dpi(260),
						forced_height = beautiful.dpi(35),
						widget = wibox.container.background,
					},
					margins = beautiful.dpi(10),
					widget = wibox.container.margin,
				},
				halign = "center",
				widget = wibox.container.place,
			},
			M.clips[1],
			M.clips[2],
			M.clips[3],
			M.clips[4],
			M.clips[5],
			spacing = beautiful.dpi(10),
			layout = wibox.layout.fixed.vertical,
		},
		forced_width = beautiful.dpi(600),
		margins = beautiful.dpi(20),
		widget = wibox.container.margin,
	})
end

M.open = function()
	M.keygrabber:start()
	M.lclip:recent()
	M.popup.visible = true

	local timed = rubato.timed({
		duration = 0.5,
		intro = 2 / 10,
		override_dt = true,
		subscribed = function(opacity)
			M.popup.opacity = opacity
		end,
	})
	timed.target = 0.8
end

M.close = function()
	M.keygrabber:stop()
	M.popup.visible = false
	M.input.text = ""
end

M.current = {}

M.keygrabber = awful.keygrabber({
	mask_modkeys = true,
	keypressed_callback = function(_, mod, key)
		if mod[1] == vars.modkey then
			if key == "c" then
				M.close()
			elseif tonumber(key) >= 1 and tonumber(key) <= 5 then
				M.lclip:select(M.current[tonumber(key)])
				M.close()
			end
		elseif mod[1] == "Mod4" then
			if tonumber(key) >= 1 and tonumber(key) <= 5 then
				M.lclip:remove(M.current[tonumber(key)])
				if M.input.text == "" then
					M.lclip:recent()
				else
					M.lclip:search(M.input.text)
				end
			end
		elseif key == "Escape" then
			M.close()
		elseif key == "BackSpace" then
			M.input.text = M.input.text:sub(1, -2)
			if M.input.text == "" then
				M.lclip:recent()
			else
				M.lclip:search(M.input.text)
			end
		elseif #key == 1 then
			M.input.text = M.input.text .. key
			M.lclip:search(M.input.text)
		end
	end,
})

M.update = function(data)
	M.current = data
	for n, s in ipairs(data) do
		M.clips[n].children[1].children[1].text = n .. ") "
		M.clips[n].children[2].children[1].text = s
	end
end

M.remove = function(string)
	M.lclip:remove(string)
end

return M

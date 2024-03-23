<h1 align="center"> AwesomeWm LuaClip</h1>
<p align="center"> Text Clipboard manager for Awesome Windows Manager </p>
<p align="center"> A quick way to retrieve previous clipboard entries inside awesome and quickly set them as the current selection, stores one hundred entries tryiing to keep it light weight and minimal.</p>

<h3>Dependencies</h3>

- AwesomeWM
- Lua
- X11
- PkgConfig
- GLib
- Threads
- XFixes
- Cmake

<h3>Installation</h3>

```bash
git clone https://github.com/awesomeWM/LuaClip.git
cd LuaClip
cmake -S . -B build
cmake --build build
```
This will give you a libluaclip.so library that can be included in your configs, placed in your root directory of your awesomeWm config is the easiest.

<h3>Usage</h3>

```lua
local luaclip = require "libluaclip"
local lclip = luaclip()

lclip:daemon()
```
Will start the daemon in a seperate thread in the background, all communication is handled with GLIb and awesomeWM messaging system.

<h3>Functions</h3>

```lua
lclip:search("search term")
--return most recent 5 matching entries or NIL if none found

lclip:recent()
--return 5 most recent entries

lclip:select("selection")
--text to enter into clipboard can be pasted normally
```


<h3>Signal</h3>

```lua
awesome.connect_signal("clipboard::selection", function(selection)
for n, s in ipairs(selection) do
    selections[n] = s
end)
```

<h3>Example</h3>

```lua
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
	font = "SFMonoNerdFontMono 14",
	widget = wibox.widget.textbox,
	text = "Clipboard: ",
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
			text = " ",
			widget = wibox.widget.textbox,
			forced_height = beautiful.dpi(70),
		})
	end

	M.popup:setup({
		{
			{
				{
					utils.add_bg(M.prompt, nil, beautiful.yellow),
					M.input,
					spacing = beautiful.dpi(10),
					layout = wibox.layout.fixed.horizontal,
				},
				margins = beautiful.dpi(10),
				widget = wibox.container.margin,
			},
			wibox.widget.separator({
				orientation = "horizontal",
				color = beautiful.magenta,
				thickness = beautiful.dpi(1),
				forced_height = beautiful.dpi(5),
				forced_width = beautiful.dpi(260),
			}),
			M.clips[1],
			M.clips[2],
			M.clips[3],
			M.clips[4],
			M.clips[5],
			spacing = beautiful.dpi(10),
			layout = wibox.layout.fixed.vertical,
		},
		forced_width = 800,
		margins = beautiful.dpi(10),
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
	timed.target = 1
end

M.close = function()
	M.keygrabber:stop()
	M.popup.visible = false
	M.input.text = ""
end

M.current = {}

M.keygrabber = awful.keygrabber({
	mask_modkeys = true,
	keybindings = {
		{
			{},
			"Escape",
			function()
				M.close()
			end,
		},
		{
			{},
			"Return",
			function()
				if M.current[1] then
					M.lclip:select(M.current[1])
					M.close()
				end
			end,
		},
		{
			{},
			"BackSpace",
			function()
				M.input.text = M.input.text:sub(1, -2)
				if M.input.text == "" then
					M.lclip:recent()
				else
					M.lclip:search(M.input.text)
				end
			end,
		},
		{
			{ vars.modkey },
			"c",
			function()
				M.close()
			end,
		},
		{
			{ vars.modkey },
			"1",
			function()
				if M.current[1] then
					M.lclip:select(M.current[1])
					M.close()
				end
			end,
		},
		{
			{ vars.modkey },
			"2",
			function()
				if M.current[2] then
					M.lclip:select(M.current[2])
					M.close()
				end
			end,
		},
		{
			{ vars.modkey },
			"3",
			function()
				if M.current[3] then
					M.lclip:select(M.current[3])
					M.close()
				end
			end,
		},
		{
			{ vars.modkey },
			"4",
			function()
				if M.current[4] then
					M.lclip:select(M.current[4])
					M.close()
				end
			end,
		},
		{
			{ vars.modkey },
			"5",
			function()
				if M.current[5] then
					M.lclip:select(M.current[5])
					M.close()
				end
			end,
		},
	},
	keypressed_callback = function(_, mod, key)
		if #key == 1 then
			M.input.text = M.input.text .. key
			M.lclip:search(M.input.text)
		end
	end,
})

awesome.connect_signal("clipboard::selection", function(data)
	M.current = data
	for n, s in ipairs(data) do
		M.clips[n].markup = "<span foreground='#e0af68'><b>" .. n .. ")  </b></span> " .. s
	end
end

return M
```

Create a popup and use keygrabber and textbox to simulate input box, any single keystroke is entered and updates search. ModKey + 1-5 makes selection or Return selects first option.



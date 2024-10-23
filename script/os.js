print("Welcome to Unnamed OS");

var Icon = {
    w: 16,
    h: 16,
    Released: 0,
    Pressed: 1,
    Pencil: 2
};

var Tab = {
    w: 8,
    h: 8,
    Sprite: 0,
    Code: 1,
    Nodes: 2,
    Tilemap: 3,
    Music: 4,
    Console: 5,
    Scheme: 6
};

var Gui = {
    active: null,
    choice: function(x, y, option, options) {
        var w = 40;
        var h = 10;
        fillRect(Persistent.screen, x, y, w, h, yellowd);
        string(Persistent.screen, Persistent.font, x, y, white, option);
        if (this.active == null && isPressed("a", x, y, w, h)) {
            this.active = option;
        } else if (!isOver(x, y, w, h * options.length)) {
            this.active = null;
        }
        if (this.active != null) {
            for (var i = 0; i < options.length; i++) {
                var o = options[i];
                fillRect(Persistent.screen, x, y + i * h, w, h, yellowd);
                string(Persistent.screen, Persistent.font, x, y + i * h, white, o);
                if (isPressed("a", x, y + i * h, w, h)) {
                    this.active = null;
                    
                    return o;
                }
            }
        }
        return option;
    }
};

var PencilSpriteTool = {
    icon: 2,
    onSelect: function() {
        
    },
    onUpdate: function(mouseX, mouseY) {
        if (isDown("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            setPixel(SpriteEditor.surface.active, mouseX + size(SpriteEditor.surface.active) * SpriteEditor.surface.sprite, mouseY, Persistent.color);
        }
    }
};

var LineSpriteTool = {
    icon: 3,
    start: null,
    end: null,
    onSelect: function() {
        this.start = null;
        this.end = null;
        SpriteEditor.surface.clone();
    },
    onUpdate: function(mouseX, mouseY) {
        SpriteEditor.surface.restore();

        var s = size(SpriteEditor.surface.active);
        var offset = s * SpriteEditor.surface.sprite;
        if (isPressed("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.start = {x: mouseX, y: mouseY};
            this.end = null;
        } else if (isReleased("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.end = {x: mouseX, y: mouseY};
            line(Persistent.buffer1, this.start.x, this.start.y, this.end.x, this.end.y, Persistent.color);
        } else if (isDown("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            line(SpriteEditor.surface.active, this.start.x + offset, this.start.y, mouseX + offset, mouseY, Persistent.color);
        }

        clearScissor(SpriteEditor.surface.active);
    }
};

var EllipseSpriteTool = {
    icon: 4,
    start: null,
    size: null,
    onSelect: function() {
        this.start = null;
        this.size = null;
        SpriteEditor.surface.clone();
    },
    onUpdate: function(mouseX, mouseY) {
        SpriteEditor.surface.restore();

        var s = size(SpriteEditor.surface.active);
        var offset = s * SpriteEditor.surface.sprite;
        if (isPressed("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.start = {x: mouseX, y: mouseY};
            this.size = null;
        } else if (isReleased("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.size = {x: Math.abs(mouseX - this.start.x), y: Math.abs(mouseY - this.start.y)};
            ellipse(Persistent.buffer1, this.start.x, this.start.y, this.size.x, this.size.y, Persistent.color);
        } else if (isDown("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.size = {x: Math.abs(mouseX - this.start.x), y: Math.abs(mouseY - this.start.y)};
            ellipse(SpriteEditor.surface.active, this.start.x + offset, this.start.y, this.size.x, this.size.y, Persistent.color);
        }

        clearScissor(SpriteEditor.surface.active);
    }
};

var FillEllipseSpriteTool = {
    icon: 5,
    start: null,
    size: null,
    onSelect: function() {
        this.start = null;
        this.size = null;
        SpriteEditor.surface.clone();
    },
    onUpdate: function(mouseX, mouseY) {
        SpriteEditor.surface.restore();

        var s = size(SpriteEditor.surface.active);
        var offset = s * SpriteEditor.surface.sprite;
        if (isPressed("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.start = {x: mouseX, y: mouseY};
            this.size = null;
        } else if (isReleased("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.size = {x: Math.abs(mouseX - this.start.x), y: Math.abs(mouseY - this.start.y)};
            fillEllipse(Persistent.buffer1, this.start.x, this.start.y, this.size.x, this.size.y, Persistent.color);
        } else if (isDown("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.size = {x: Math.abs(mouseX - this.start.x), y: Math.abs(mouseY - this.start.y)};
            fillEllipse(SpriteEditor.surface.active, this.start.x + offset, this.start.y, this.size.x, this.size.y, Persistent.color);
        }

        clearScissor(SpriteEditor.surface.active);
    }
};

var RectangleSpriteTool = {
    icon: 6,
    start: null,
    size: null,
    onSelect: function() {
        this.start = null;
        this.size = null;
        SpriteEditor.surface.clone();
    },
    onUpdate: function(mouseX, mouseY) {
        SpriteEditor.surface.restore();

        var s = size(SpriteEditor.surface.active);
        var offset = s * SpriteEditor.surface.sprite;
        if (isPressed("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.start = {x: mouseX, y: mouseY};
            this.size = null;
        } else if (isReleased("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.size = {x: mouseX - this.start.x, y: mouseY - this.start.y};
            rect(Persistent.buffer1, this.start.x, this.start.y, this.size.x, this.size.y, 1, Persistent.color);
        } else if (isDown("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.size = {x: mouseX - this.start.x, y: mouseY - this.start.y};
            rect(SpriteEditor.surface.active, this.start.x + offset, this.start.y, this.size.x, this.size.y, 1, Persistent.color);
        }

        clearScissor(SpriteEditor.surface.active);
    }
};

var FillRectangleSpriteTool = {
    icon: 7,
    start: null,
    size: null,
    onSelect: function() {
        this.start = null;
        this.size = null;
        SpriteEditor.surface.clone();
    },
    onUpdate: function(mouseX, mouseY) {
        SpriteEditor.surface.restore();

        var s = size(SpriteEditor.surface.active);
        var offset = s * SpriteEditor.surface.sprite;
        if (isPressed("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.start = {x: mouseX, y: mouseY};
            this.size = null;
        } else if (isReleased("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.size = {x: mouseX - this.start.x, y: mouseY - this.start.y};
            fillRect(Persistent.buffer1, this.start.x, this.start.y, this.size.x, this.size.y, Persistent.color);
        } else if (isDown("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            this.size = {x: mouseX - this.start.x, y: mouseY - this.start.y};
            fillRect(SpriteEditor.surface.active, this.start.x + offset, this.start.y, this.size.x, this.size.y, Persistent.color);
        }

        clearScissor(SpriteEditor.surface.active);
    }
};

var BucketSpriteTool = {
    icon: 8,
    onSelect: function() {
        
    },
    onUpdate: function(mouseX, mouseY) {
        if (isPressed("a", SpriteEditor.surface.x, SpriteEditor.surface.y, SpriteEditor.surface.w, SpriteEditor.surface.h)) {
            var s = size(SpriteEditor.surface.active);
            var offset = s * SpriteEditor.surface.sprite;
            fillRect(SpriteEditor.surface.active, offset, 0, s, s, Persistent.color);
        }
    }
};

var SpriteEditor = {
    background: 14,
    toolbar: {
        x: 1,
        y: 12,
        active: PencilSpriteTool,
        list: [
            PencilSpriteTool,
            LineSpriteTool,
            EllipseSpriteTool,
            FillEllipseSpriteTool,
            RectangleSpriteTool,
            FillRectangleSpriteTool,
            BucketSpriteTool
        ]
    },
    surface: {
        x: 75,
        y: 43,
        w: 64,
        h: 64,
        sprite: 1,
        active: 2,
        clone: function() {
            clearScissor(Persistent.buffer1);
            clear(Persistent.buffer1);
            scissorAtlas(SpriteEditor.surface.active, SpriteEditor.surface.sprite);
            blit(Persistent.buffer1, SpriteEditor.surface.active, 0, 0);
            clearScissor(SpriteEditor.surface.active);
        },
        restore: function() {
            scissorAtlas(SpriteEditor.surface.active, SpriteEditor.surface.sprite);
            clear(SpriteEditor.surface.active);

            var s = size(SpriteEditor.surface.active);
            scissor(Persistent.buffer1, 0, 0, s, s);
            blitAtlas(SpriteEditor.surface.active, Persistent.buffer1, SpriteEditor.surface.sprite);
            clearScissor(Persistent.buffer1);
        }
    }
};

var PencilTilemapTool = {
    icon: 2,
    onSelect: function() {
        
    },
    onUpdate: function(mouseX, mouseY) {
        if (isDown("a", TilemapEditor.surface.x, TilemapEditor.surface.y, TilemapEditor.surface.w, TilemapEditor.surface.h)) {
            setTile(TilemapEditor.surface.active, TilemapEditor.surface.sprite, mouseX, mouseY);
        }
    }
};

var TilemapEditor = {
    background: 15,
    toolbar: {
        x: 1,
        y: 12,
        active: PencilTilemapTool,
        list: [
            PencilTilemapTool
        ]
    },
    surface: {
        x: 0,
        y: 43,
        w: 214,
        h: 64,
        sprite: 1,
        active: 21
    },
    atlas: 2
};

var SpriteTab = {
    icon: Tab.Sprite,
    onClick: function() {
        Persistent.editor = SpriteEditor;
    }
};

var CodeTab = {
    icon: Tab.Code,
    onClick: function() {
        
    }
};

var NodesTab = {
    icon: Tab.Nodes,
    onClick: function() {
        
    }
};

var TilemapTab = {
    icon: Tab.Tilemap,
    onClick: function() {
        Persistent.editor = TilemapEditor;
    }
};

var MusicTab = {
    icon: Tab.Music,
    onClick: function() {
        
    }
};

var ConsoleTab = {
    icon: Tab.Console,
    onClick: function() {
        
    }
};

var SchemeTab = {
    icon: Tab.Scheme,
    onClick: function() {
        
    }
};


var Persistent = {
    screen: 0,
    buffer1: 11,
    buffer2: 12,
    buffer3: 13,
    font: 1,
    icons: 3,
    tabs: 4,
    color: red,
    editor: SpriteEditor,
    top: {
        x: 1,
        y: 1,
        active: SpriteTab,
        list: [
            SpriteTab,
            CodeTab,
            NodesTab,
            TilemapTab,
            MusicTab,
            ConsoleTab,
            SchemeTab
        ]
    }
};






function onAdd() {

}

function onRemove() {
    
}

function onUpdate() {
    clear(Persistent.screen);

    blit(0, Persistent.editor.background, 0, 0);

    // Draw tabs
    for (var i = 0; i < Persistent.top.list.length; i++) {
        var tab = Persistent.top.list[i];
        var x = Persistent.top.x + i * Tab.w + i;
        var y = Persistent.top.y;
        scissorAtlas(Persistent.tabs, tab.icon);
        blit(Persistent.screen, Persistent.tabs, x, y);
        if (isPressed("a", x, y, Tab.w, Tab.h)) {
            tab.onClick();
        }
    }
    clearScissor(Persistent.tabs);

    if (Persistent.editor === SpriteEditor) {
        // Draw toolbar
        for (var i = 0; i < SpriteEditor.toolbar.list.length; i++) {
            var tool = SpriteEditor.toolbar.list[i];
            var x = SpriteEditor.toolbar.x + i * Icon.w + i;
            var y = SpriteEditor.toolbar.y;
            if (isPressed("a", x, y, Icon.w, Icon.h)) {
                SpriteEditor.toolbar.active = tool;
                tool.onSelect();
            }
            if (SpriteEditor.toolbar.active === tool) {
                scissorAtlas(Persistent.icons, Icon.Pressed);
                blit(Persistent.screen, Persistent.icons, x, y);
            } else {
                scissorAtlas(Persistent.icons, Icon.Released);
                blit(Persistent.screen, Persistent.icons, x, y);
            }
            scissorAtlas(Persistent.icons, tool.icon);
            blit(Persistent.screen, Persistent.icons, x, y);
        }
        clearScissor(Persistent.icons);
    
        // Draw canvas
        var s = size(SpriteEditor.surface.active);
        var scaleX = SpriteEditor.surface.w / s;
        var scaleY = SpriteEditor.surface.h / s;
        for (var dx = 0; dx < s; dx++) {
            for (var dy = 0; dy < s; dy++) {
                fillRect(Persistent.screen,
                    SpriteEditor.surface.x + dx * scaleX,
                    SpriteEditor.surface.y + dy * scaleY,
                    scaleX, scaleY,
                    getPixel(SpriteEditor.surface.active, dx + s * SpriteEditor.surface.sprite, dy)
                );
            }
        }
    
        // Draw palette
        {
            var x = 163;
            var y = 57;
            var scale = 8;
            for (var i = 0; i < palette.length; i++) {
                var dx = i % 4;
                var dy = Math.floor(i / 4);
                var color = palette[i];
                fillRect(Persistent.screen, x + dx * scale, y + dy * scale, scale, scale, color);
                if (isPressed("a", x + dx * scale, y + dy * scale, scale, scale)) {
                    Persistent.color = color;
                }
            }
        }
        
        // Draw atlas
        {
            var c = count(SpriteEditor.surface.active);
            var s = size(SpriteEditor.surface.active);
            var x = 1, y = 116;
            var row = (width - 2) / (s + 1);
            for (var i = 0; i < c; i++) {
                var dx = i % row;
                var dy = Math.floor(i / row);
                fillRect(Persistent.screen, x + dx * s + dx, y + dy * s + dy, s, s, dark);
                scissorAtlas(SpriteEditor.surface.active, i);
                blit(Persistent.screen, SpriteEditor.surface.active, x + dx * s + dx, y + dy * s + dy);
                if (isPressed("a", x + dx * s + dx, y + dy * s + dy, s, s)) {
                    SpriteEditor.surface.sprite = i;
                }
            }
            clearScissor(SpriteEditor.surface.active);
        }
    
        // Atlas choice
        SpriteEditor.surface.active = Gui.choice(1, 103, SpriteEditor.surface.active, [2, 3, 4, 5]);
    
        // Update tool
        var s = size(SpriteEditor.surface.active);
        var mouseX = Math.floor(Math.clamp(0, s - 1, Math.lerp(SpriteEditor.surface.x, 0, SpriteEditor.surface.x + SpriteEditor.surface.w, s, mx)));
        var mouseY = Math.floor(Math.clamp(0, s - 1, Math.lerp(SpriteEditor.surface.y, 0, SpriteEditor.surface.y + SpriteEditor.surface.h, s, my)));
        SpriteEditor.toolbar.active.onUpdate(mouseX, mouseY);
    }

    if (Persistent.editor === TilemapEditor) {
        // Draw toolbar
        for (var i = 0; i < TilemapEditor.toolbar.list.length; i++) {
            var tool = TilemapEditor.toolbar.list[i];
            var x = TilemapEditor.toolbar.x + i * Icon.w + i;
            var y = TilemapEditor.toolbar.y;
            if (isPressed("a", x, y, Icon.w, Icon.h)) {
                TilemapEditor.toolbar.active = tool;
                tool.onSelect();
            }
            if (TilemapEditor.toolbar.active === tool) {
                scissorAtlas(Persistent.icons, Icon.Pressed);
                blit(Persistent.screen, Persistent.icons, x, y);
            } else {
                scissorAtlas(Persistent.icons, Icon.Released);
                blit(Persistent.screen, Persistent.icons, x, y);
            }
            scissorAtlas(Persistent.icons, tool.icon);
            blit(Persistent.screen, Persistent.icons, x, y);
        }
        clearScissor(Persistent.icons);

        // Draw canvas
        clear(TilemapEditor.surface.active);
        blitTiles(TilemapEditor.surface.active, TilemapEditor.atlas);
        scissor(Persistent.screen, TilemapEditor.surface.x, TilemapEditor.surface.y, TilemapEditor.surface.w, TilemapEditor.surface.h);
        blit(Persistent.screen, TilemapEditor.surface.active, TilemapEditor.surface.x, TilemapEditor.surface.y);
        clearScissor(Persistent.screen);

        // Draw atlas
        {
            var c = count(TilemapEditor.atlas);
            var s = size(TilemapEditor.atlas);
            var x = 1, y = 116;
            var row = (width - 2) / (s + 1);
            for (var i = 0; i < c; i++) {
                var dx = i % row;
                var dy = Math.floor(i / row);
                fillRect(Persistent.screen, x + dx * s + dx, y + dy * s + dy, s, s, dark);
                scissorAtlas(TilemapEditor.atlas, i);
                blit(Persistent.screen, TilemapEditor.atlas, x + dx * s + dx, y + dy * s + dy);
                if (isPressed("a", x + dx * s + dx, y + dy * s + dy, s, s)) {
                    TilemapEditor.surface.sprite = i;
                }
            }
            clearScissor(TilemapEditor.atlas);
        }

        // Atlas choice
        TilemapEditor.atlas = Gui.choice(1, 103, TilemapEditor.atlas, [2, 3, 4, 5]);

        // Update tool
        var s = size(TilemapEditor.atlas);
        var mouseX = Math.floor(Math.lerp(TilemapEditor.surface.x, 0, TilemapEditor.surface.x + TilemapEditor.surface.w, TilemapEditor.surface.w / s, mx));
        var mouseY = Math.floor(Math.lerp(TilemapEditor.surface.y, 0, TilemapEditor.surface.y + TilemapEditor.surface.h, TilemapEditor.surface.h / s, my));
        TilemapEditor.toolbar.active.onUpdate(mouseX, mouseY);
    }
}
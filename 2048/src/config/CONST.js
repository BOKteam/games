/**
 * Created by Envee.
 *
 * Date: 14-10-20
 * Time: 下午6:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

var CONST = {
    BG: {
        SEGMENT: 150,
        WIDTH: 540,
        HEIGHT: 960
    },
    GRID: {
        WIDTH: 400,
        HEIGHT: 400,
        RADIUS: 5,
        ROW_RADIUS: 3,
        ROW: 4,
        COLUMN: 4,
        DISTANCE: 12,
        TILE_HIDE_TIME: 350,
        TILE_SHOW_TIME: 200,
        TILE_MOVE_TIME: 200
    },
    TILE: {
        STYLE: {
            "tile-2": {background: '#eee4da', color: "#776E65", "shadow": {color: "rgba(243, 215, 116, 0)", offsetX: "5", offsetY: "5"}, "fontSize": "35px"},
            "tile-4": {background: '#ede0c8', color: "#776E65", "shadow": {color: "rgba(243, 215, 116, 0)", offsetX: "5", offsetY: "5"}, "fontSize": "35px"},
            "tile-8": {background: '#f2b179', color: "#f9f6f2", "shadow": {color: "rgba(243, 215, 116, 0)", offsetX: "5", offsetY: "5"}, "fontSize": "35px"},
            "tile-16": {background: '#f59563', color: "#f9f6f2", "shadow": {color: "rgba(243, 215, 116, 0)", offsetX: "5", offsetY: "5"}, "fontSize": "35px"},
            "tile-32": {background: '#f67c5f', color: "#f9f6f2", "shadow": {color: "rgba(243, 215, 116, 0)", offsetX: "5", offsetY: "5"}, "fontSize": "35px"},
            "tile-64": {background: '#f65e3b', color: "#f9f6f2", "shadow": {color: "rgba(243, 215, 116, 0)", offsetX: "5", offsetY: "5"}, "fontSize": "35px"},
            "tile-128": {background: '#edcf72', color: "#f9f6f2", "shadow": {color: "rgba(243, 215, 116, 0.31746)", offsetX: "5", offsetY: "5"}, "fontSize": "25px"},
            "tile-256": {background: '#edcc61', color: "#f9f6f2", "shadow": {color: "rgba(243, 215, 116, 0.31746)", offsetX: "5", offsetY: "5"}, "fontSize": "25px"},
            "tile-512": {background: '#edc850', color: "#f9f6f2", "shadow": {color: "rgba(243, 215, 116, 0.47619)", offsetX: "5", offsetY: "5"}, "fontSize": "25px"},
            "tile-1024": {background: '#edc53f', color: "#f9f6f2", "shadow": {color: "rgba(243, 215, 116, 0.47619)", offsetX: "5", offsetY: "5"}, "fontSize": "15px"},
            "tile-2048": {background: '#edc22e', color: "#f9f6f2", "shadow": {color: "rgba(243, 215, 116, 0.55556)", offsetX: "5", offsetY: "5"}, "fontSize": "15px"},
            "tile-super": {background: '#3c3a32', color: "#f9f6f2", "shadow": {color: "rgba(243, 215, 116, 0.55556)", offsetX: "5", offsetY: "5"}, "fontSize": "15px"}
        }
    },
    GAME_PLAY: {
        START_SIZE: 2
    }
};
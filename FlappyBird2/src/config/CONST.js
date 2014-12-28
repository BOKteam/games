/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 下午1:38
 * Write the description in this section.
 */

var CONST = {
    CANVAS: {
        WIDTH: 900,
        HEIGHT: 600
    },
    BG:{
        HEIGHT: 504
    },
    BIRD: {
        WIDTH: 85,
        HEIGHT: 60,
        WIDTHLEAN: 68,//倾斜时的宽度
        HEIGHTLEAN: 100,//倾斜时的高度
        HEIGHTVERTICAL: 85,// 倾斜90DEG时高度

        OFFSETX: 30,
        FACE_ROTATE_MAX: 90,
        FACE_ROTATE_MIN: -30
    },
    GAME_PLAY: {
        FLAP_SPEED: -10,
        GRAVITY: 1,
        SCROLL_SPEED: 5
    },
    PIPE: {
        WIDTH: 148,
        HEIGHT: 1664,
        APART: 140
}
};
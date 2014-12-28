/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午11:00
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("tzfe.features.game.v.component.Tile");
goog.require("org.createjs.easeljs.EaselJS");

tzfe.features.game.v.component.Tile = Tile;
BOK.inherits(Tile, createjs.Container);

function Tile(position, value) {
    createjs.Container.call(this);

    this.row = position.row;
    this.col = position.col;
    this.value = value || 2;
    this.previousPosition = null;
    this.mergedFrom = null;
    this.nextPosition = null;

    var columnWidth = (CONST.GRID.WIDTH - (CONST.GRID.DISTANCE * (CONST.GRID.COLUMN + 1))) / CONST.GRID.COLUMN;
    var columnHeight = (CONST.GRID.HEIGHT - (CONST.GRID.DISTANCE * (CONST.GRID.ROW + 1))) / CONST.GRID.ROW;

    var valueObj = CONST.TILE.STYLE['tile-' + value];
    var text = new createjs.Text(value, "bold " + valueObj.fontSize + " ClearSans", valueObj.color).set({x: columnWidth /2, y: columnHeight / 2, textAlign: 'center', textBaseline:'middle' });
    var tileS = new createjs.Shape(new createjs.Graphics().beginFill(valueObj.background).drawRoundRect(0, 0, columnWidth, columnHeight, CONST.GRID.ROW_RADIUS));
    tileS.set({shadow: new createjs.Shadow(valueObj.shadow.color, valueObj.shadow.offsetX, valueObj.shadow.offsetY)});
    this.addChild(tileS);
    this.addChild(text);
}

Tile.prototype.savePosition = function () {
    this.previousPosition = { row: this.row, col: this.col };
};

Tile.prototype.updatePosition = function (position) {
    this.row = position.row;
    this.col = position.col;
};

Tile.prototype.serialize = function () {
    return {
        position: {
            row: this.row,
            col: this.col
        },
        value: this.value
    };
};
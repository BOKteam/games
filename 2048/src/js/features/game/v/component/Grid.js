/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午9:08
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("tzfe.features.game.v.component.Grid");
goog.require("org.createjs.easeljs.EaselJS");
goog.require("bok.util.EaselAnimationHelper");

tzfe.features.game.v.component.Grid = Grid;
BOK.inherits(Grid, createjs.Container);

function Grid() {
    createjs.Container.call(this);

    this.cells = this.empty();
    this.addChild(new createjs.Shape(new createjs.Graphics().beginFill('#BBADA0').drawRoundRect(0, 0, CONST.GRID.WIDTH, CONST.GRID.HEIGHT, CONST.GRID.RADIUS)));
    this.columnWidth = (CONST.GRID.WIDTH - (CONST.GRID.DISTANCE * (CONST.GRID.COLUMN + 1))) / CONST.GRID.COLUMN;
    this.columnHeight = (CONST.GRID.HEIGHT - (CONST.GRID.DISTANCE * (CONST.GRID.ROW + 1))) / CONST.GRID.ROW;
    //add back cell
    var iOffSetX = CONST.GRID.DISTANCE;
    var iOffSetY = CONST.GRID.DISTANCE;
    for (var row = 0; row < CONST.GRID.ROW; row++) {
        this.cells[row] = [];
        for (var col = 0; col < CONST.GRID.COLUMN; col++) {
            this.addChild(new createjs.Shape(new createjs.Graphics().beginFill('#CCC0B3').drawRoundRect(iOffSetX, iOffSetY, this.columnWidth, this.columnHeight, CONST.GRID.ROW_RADIUS)));
            this.cells[row][col] = {x: iOffSetX, y: iOffSetY, tile: null};
            iOffSetX += this.columnWidth + CONST.GRID.DISTANCE;
        }
        iOffSetX = CONST.GRID.DISTANCE;
        iOffSetY += this.columnHeight + CONST.GRID.DISTANCE;
    }

}
Grid.prototype.update = function () {

};
Grid.prototype.empty = function () {
    var cells = [];

    for (var x = 0; x < CONST.GRID.ROW; x++) {
        var row = cells[x] = [];

        for (var y = 0; y < CONST.GRID.COLUMN; y++) {
            row.push(null);
        }
    }
    return cells;
};

Grid.prototype.insertTile = function (tile) {
    this.cells[tile.row][tile.col].tile = tile;
    tile.set({x: this.cells[tile.row][tile.col].x,
        y: this.cells[tile.row][tile.col].y,
        scaleX: 0,
        scaleY: 0
    });
    this.addChild(tile);
    createjs.Tween.get(tile).to({scaleX: 1, scaleY: 1}, CONST.GRID.TILE_HIDE_TIME, createjs.Ease.bounceOut);
};
Grid.prototype.clearTiles = function(){
    for (var row = 0; row < CONST.GRID.ROW; row++) {
        for (var col = 0; col < CONST.GRID.COLUMN; col++) {
            if(this.cells[row][col].tile){
                this.removeTile(this.cells[row][col].tile);
            }
        }
    }
};

Grid.prototype.removeTile = function (tile) {
    var cell = this.cells[tile.row][tile.col];
    EaselAnimationHelper.disappear(tile, CONST.GRID.TILE_SHOW_TIME).call(Delegate.create(this, function(){
        this.removeChild(tile);
    }));
    this.cells[tile.row][tile.col].tile = null;
};

Grid.prototype.moveNewTile = function (tile, cell) {
    this.cells[tile.row][tile.col].tile = null;
    tile.updatePosition(cell);
    this.cells[cell.row][cell.col].tile = tile;
    EaselAnimationHelper.bounceTo(tile, this.getPosition(cell).x, this.getPosition(cell).y, CONST.GRID.TILE_SHOW_TIME);
};
Grid.prototype.getPosition = function (cell) {
    return this.cells[cell.row][cell.col];
};
Grid.prototype.randomAvailableCell = function () {
    var cells = this.availableCells();

    if (cells.length) {
        return cells[Math.floor(Math.random() * cells.length)];
    }
};

Grid.prototype.availableCells = function () {
    var cells = [];

    this.eachCell(function (row, col, cell) {
        if (!cell.tile) {
            cells.push({ row: row, col: col});
        }
    });

    return cells;
};

/**
 *
 * @param {Function} callback   //the callback function
 */
Grid.prototype.eachCell = function (callback) {
    //add back cell
    for (var row = 0; row < CONST.GRID.ROW; row++) {
        for (var col = 0; col < CONST.GRID.COLUMN; col++) {
            callback(row, col, this.cells[row][col]);
        }
    }
};

// Check if there are any cells available
Grid.prototype.cellsAvailable = function () {
    return !!this.availableCells().length;
};

Grid.prototype.cellAvailable = function (cell) {
    if (!this.cells[cell.row][cell.col].tile) {
        return this.cells[cell.row][cell.col];
    } else {
        return null;
    }
};

Grid.prototype.getCellContent = function (cell) {
    if (this.isCellInBounds(cell)) {
        return this.cells[cell.row][cell.col];
    } else {
        return null;
    }
};

Grid.prototype.isCellInBounds = function (cell) {
    return cell.row >= 0 && cell.row < CONST.GRID.ROW &&
        cell.col >= 0 && cell.col < CONST.GRID.COLUMN;
};

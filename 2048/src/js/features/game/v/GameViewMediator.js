/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午9:08
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("tzfe.features.game.v.GameViewMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("org.createjs.easeljs.EaselJS");
goog.requireAll("tzfe.features.game.v.component.*");


tzfe.features.game.v.GameViewMediator = GameViewMediator;
BOK.inherits(GameViewMediator, BaseMediator);

function GameViewMediator(stage) {
    BaseMediator.call(this);

    this.score = 0;
    this.over = false;
    this.bg = new createjs.Shape(new createjs.Graphics().beginFill("#FAF8EF").drawRect(0, 0, CONST.BG.WIDTH, CONST.BG.HEIGHT));

    this.logo = new createjs.Text("2048", "bold 50px ClearSans", "#776E65").set({x: CONST.BG.WIDTH / 2 - CONST.GRID.WIDTH / 2, y: 10});
    this.mainGrid = new tzfe.features.game.v.component.Grid();

    this.scoreLabel = new tzfe.features.game.v.component.Label(50, 40, "SCORE", 0);
    this.scoreLabel.set({x: CONST.BG.WIDTH / 2, y: 10});

    this.bestLabel = new tzfe.features.game.v.component.Label(140, 40, "BEST", localStorage.bestScore);
    this.bestLabel.set({x: CONST.BG.WIDTH / 2 + 60, y: 10});

    this.btnNewGame = new tzfe.features.game.v.component.Button(140, 30, "New Game");
    this.btnNewGame.set({x: CONST.BG.WIDTH / 2 + 60, y: 70});

    this.btnNewGame.addEventListener('click', Delegate.create(this, this.startNewGame_));

    this.mainGrid.set({x: CONST.BG.WIDTH / 2 - CONST.GRID.WIDTH / 2, y: CONST.BG.HEIGHT / 2 - CONST.GRID.HEIGHT / 2 + 40 });
   
    this.gameOver = new tzfe.features.game.v.component.GameOver();
    this.gameOver.set({x: CONST.BG.WIDTH / 2 - CONST.GRID.WIDTH / 2, y: CONST.BG.HEIGHT / 2 - CONST.GRID.HEIGHT / 2 + 40 });
    this.gameOver.buttonReplay.addEventListener('click', Delegate.create(this, this.startNewGame_));
    this.gameOver.visible = false;

    stage.addChild(this.bg, this.logo, this.mainGrid, this.scoreLabel, this.bestLabel, this.btnNewGame, this.gameOver);
    this.addStartTiles();

    // Respond to direction keys
    document.addEventListener("keydown", Delegate.create(this, this.onKeyDown_));
}
GameViewMediator.prototype.startNewGame_ = function () {
    this.score = 0;
    this.over = false;
    this.gameOver.visible = false;
    this.scoreLabel.updateScore(this.score);
    this.mainGrid.clearTiles();
    this.addStartTiles();
};

GameViewMediator.prototype.onKeyDown_ = function (event) {
    var map = {
        38: 0, // Up
        39: 1, // Right
        40: 2, // Down
        37: 3, // Left
        75: 0, // Vim up
        76: 1, // Vim right
        74: 2, // Vim down
        72: 3, // Vim left
        87: 0, // W
        68: 1, // D
        83: 2, // S
        65: 3  // A
    };
    var modifiers = event.altKey || event.ctrlKey || event.metaKey ||
        event.shiftKey;
    var mapped = map[event.which];

    if (!modifiers) {
        if (mapped !== undefined) {
            event.preventDefault();
            this.move(mapped);
        }
    }

    // R key restarts the game
    if (!modifiers && event.which === 82) {
        this.restart.call(this, event);
    }
};
GameViewMediator.prototype.move = function (direction) {

    var vector = this.getVector(direction);
    var moved = false;
    var traversals = this.buildTraversals(vector);
    var megeValue = 0;
    BOK.each(traversals.rows, function (row) {
            BOK.each(traversals.cols, function (col) {
            var merged = null;
                var cell = {row: row, col: col};
                var tile = this.mainGrid.getCellContent(cell).tile;
                if (tile) {
                    var position = this.findVectorPosition(cell, vector);
                    var nextCell = this.mainGrid.getCellContent(position.next);
                    var nextCellTile = !nextCell ? nextCell : nextCell.tile;

                    if (nextCellTile && tile.value === nextCellTile.value && !nextCellTile.mergedFrom) {
                        merged = new tzfe.features.game.v.component.Tile(position.next, tile.value * 2);
                        merged.mergedFrom = [tile, nextCellTile];
                        this.score += merged.value;
                        megeValue += merged.value;
                        createjs.Tween.removeTweens(tile);
                        createjs.Tween.removeTweens(nextCellTile);
                        createjs.Tween.get(tile).to({x: this.mainGrid.getPosition(merged).x, y: this.mainGrid.getPosition(merged).y}, CONST.GRID.TILE_MOVE_TIME);
                        createjs.Tween.get(nextCellTile).to({x: this.mainGrid.getPosition(merged).x, y: this.mainGrid.getPosition(merged).y}, CONST.GRID.TILE_MOVE_TIME);
                       
                        this.mainGrid.removeTile(tile);
                        this.mainGrid.removeTile(nextCellTile);
                        this.mainGrid.insertTile(merged);
                    } else {
                        this.mainGrid.moveNewTile(tile, position.farthest);

                    }
                    if (!this.positionsEqual(cell, position.farthest) || merged) {
                        moved = true;
                        //self.addRandomTile(); // The tile moved from its original cell!
                    }

                }
                if (merged) {
                    merged.mergedFrom = null;
                }
            }, this);
        }, this
    );
    if (moved) {

        this.addRandomTile();
        if (!this.movesAvailable()) {
            this.over = true; // Game over!
        }
        if (megeValue > 0) {
            this.scoreLabel.updateScore(this.score, megeValue);
        }
        if (!localStorage.bestScore) localStorage.bestScore = 0;
        if (localStorage.bestScore < this.score) {
            localStorage.bestScore = this.score;
            this.bestLabel.updateScore(this.score);
        }
        if (this.over) {
           this.gameOver.visible = true;
        }
    }

};

GameViewMediator.prototype.findVectorPosition = function (cell, vector) {
    var previous;

    // Progress towards the vector direction until an obstacle is found
    do {
        previous = cell;
        cell = { row: previous.row + vector.x, col: previous.col + vector.y, tile: null };
    } while (this.mainGrid.isCellInBounds(cell) && this.mainGrid.cellAvailable(cell));

    return {
        farthest: previous,
        next: cell // Used to check if a merge is required
    };
};

GameViewMediator.prototype.movesAvailable = function () {
    return this.mainGrid.cellsAvailable() || this.tileMatchesAvailable();
};

GameViewMediator.prototype.tileMatchesAvailable = function () {
    for (var row = 0; row < CONST.GRID.ROW; row++) {
        for (var col = 0; col < CONST.GRID.COLUMN; col++) {
            var tile;
            tile = this.mainGrid.getCellContent({ row: row, col: col }).tile;

            if (tile) {
                for (var direction = 0; direction < 4; direction++) {
                    var vector = this.getVector(direction);
                    var cell = { row: row + vector.x, col: col + vector.y };
                    var other = this.mainGrid.getCellContent(cell);

                    if (other && other.tile && other.tile.value === tile.value) {
                        return true; // These two tiles can be merged
                    }
                }
            }
        }
    }
    return false;
};
GameViewMediator.prototype.buildTraversals = function (vector) {
    var traversals = { rows: [], cols: [] };
    for (var pos = 0; pos < CONST.GRID.COLUMN; pos++) {
        traversals.rows.push(pos);
        traversals.cols.push(pos);
    }

    // Always traverse from the farthest cell in the chosen direction
    if (vector.x === 1) traversals.rows = traversals.rows.reverse();
    if (vector.y === 1) traversals.cols = traversals.cols.reverse();

    return traversals;
};
// Get the vector representing the chosen direction
GameViewMediator.prototype.getVector = function (direction) {
    // Vectors representing tile movement
    var map = {
        0: { x: -1, y: 0 }, // Up
        1: { x: 0, y: 1 },  // Right
        2: { x: 1, y: 0 },  // Down
        3: { x: 0, y: -1 }   // Left
    };

    return map[direction];
};

GameViewMediator.prototype.addStartTiles = function () {
    for (var i = 0; i < CONST.GAME_PLAY.START_SIZE; i++) {
        this.addRandomTile();
    }
};

GameViewMediator.prototype.addRandomTile = function () {
    if (this.mainGrid.cellsAvailable()) {
        var value = Math.random() < 0.9 ? 2 : 4;
        var tile = new tzfe.features.game.v.component.Tile(this.mainGrid.randomAvailableCell(), value);

        this.mainGrid.insertTile(tile);
    }
};


GameViewMediator.prototype.positionsEqual = function (first, second) {
    return first.row === second.row && first.col === second.col;
};
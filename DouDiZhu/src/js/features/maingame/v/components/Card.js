/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-7-9
 * Time: 下午2:22
 * Write the description in this section.
 */
goog.provide("doudizhu.features.maingame.v.components.Card");
goog.require("org.createjs.easeljs.EaselJS");

(function(){

    doudizhu.features.maingame.v.components.Card = Card;

    BOK.inherits(Card, createjs.Container);
    /**
     * @param {number|String} value The valud of card, range 1-54 or the type of a card in string
     * @constructor
     * */
    function Card(value) {
        createjs.Container.call(this);

        if('string' == typeof value) {
            var index = BOK.findInArray(CardType, value);
            if(-1 == index)
                value = 0;
            else
                value = index;
        }
        else
            value || (value = 0);
        this.value = CardType[value];

        this.back_ = new createjs.Bitmap('assets/img/GameUI/back.png');
        this.face_ = value ? new createjs.Bitmap('assets/img/GameUI/c' + value + '.png') : this.back_;
        this.root_ = new createjs.Container();

        this.disabled_ = true;
        this.face_.visible = false;

        this.root_.addChild(this.face_);
        this.root_.addChild(this.back_);
        this.addChild(this.root_);

        this.addEventListener('click', Delegate.create(this, this.onClicked_));
    }

    Card.prototype.show = function() {
        this.back_.visible = false;
        this.face_.visible = true;
    };

    Card.prototype.hide = function() {
        this.face_.visible = false;
        this.back_.visible = true;
    };

    Card.prototype.enable = function() {
        this.disabled_ = false;
    };

    Card.prototype.disable = function() {
        this.disabled_ = true;
        this.unselect();
    };

    Card.prototype.isSelected = function() {
        return this.selected_;
    };

    Card.prototype.unselect = function(skipAnim) {
        if(this.selected_) {
            if(skipAnim)
                this.root_.y = 0;
            else
                createjs.Tween.get(this.root_).to({y:0}, 240);
            this.selected_ = false;
        }
    };

    Card.prototype.select = function(skipAnim) {
        if(!this.selected_) {
            if(skipAnim)
                this.root_.y = -20;
            else
                createjs.Tween.get(this.root_).to({y:-20}, 240);
            this.selected_ = true;
        }
    };

    Card.prototype.selectByPlayer = function() {
        if(!this.selected_) {
            this.dispatchEvent(new createjs.Event('selected'));
            this.select();
        }
    };

    Card.prototype.unselectByPlayer = function() {
        if(this.selected_) {
            this.unselect();
            this.dispatchEvent(new createjs.Event('unselected'));
        }
    };

    Card.prototype.onClicked_ = function() {
        if(this.disabled_)
            return;

        if(this.selected_) {
            this.unselectByPlayer();
        } else {
            this.selectByPlayer();
        }
    };

})();

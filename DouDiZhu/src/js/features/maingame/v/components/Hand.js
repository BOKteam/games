/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-7-9
 * Time: 下午2:56
 * Write the description in this section.
 */

goog.provide("doudizhu.features.maingame.v.components.Hand");

goog.require("org.createjs.tweenjs.TweenJS");
goog.require("doudizhu.features.maingame.v.components.Card");
goog.require("doudizhu.helper.Card");

(function(){
doudizhu.features.maingame.v.components.Hand = Hand;

BOK.inherits(Hand, createjs.Container);
/**
 * @param {number} side The side of a hand, range 0-2, position 0 is facing player.
 * @param {string=} name (optional) if set it should be one of 'banker', 'player1', 'player2'
 * @constructor
 * */
function Hand(side, name) {
    createjs.Container.call(this);

    this.name = name;
    this.side_ = side;
    /**@type {Array} contains {Card} */
    this.cards_ = [];
    /**@type {Object} contains {Card} */
    this.cardsIndex_ = {};
    /**@type {Array} contains {string} */
    this.selectedCards_ = [];
    /**@type {Function}  */
    this.cardSelectListener_ = Delegate.create(this, this.onCardSelected_);
    this.cardUnselectListener_ = Delegate.create(this, this.onCardUnselected_);

    switch(side) {
        case 0:
            this.stepY = 0;
            this.stepX = 35;
            break;

        case 1:
            this.stepY = 12;
            this.stepX = 0;
            break;

        case 2:
            this.stepY = 12;
            this.stepX = 0;
            break;
    }
}

/**
 * @param {Card} card
 * */
Hand.prototype.addCard = function(card) {
    card.disable();

    //add card into hand
    this.cardsIndex_[card.value] = card;
    card.addEventListener('selected', this.cardSelectListener_);
    card.addEventListener('unselected', this.cardUnselectListener_);

    //display card in correct postion
    var showCard, skew, offset = this.cards_.length;
    switch(this.side_) {
        case 0:
            showCard = true;
            skew = 0;
            break;

        case 1:
            showCard = false;
            skew = 0;
            break;

        case 2:
            showCard = false;
            skew = 0;
            break;
    }
    createjs.Tween.get(card).to({y:this.y + offset * this.stepY, x:this.x + offset * this.stepX}, 500, createjs.Ease.cubicIn).call(Delegate.create(this, function(){
        card.parent.setChildIndex(card, card.parent.children.length-1);

        //set skew to make cards looking 3D
        card.skewY = skew;

        //add card to hand
        CardHelper.insertCardToHand(card, this.cards_, function(c){
            return c.value;
        });

        //show card if hand facing player
        if(showCard) {
            card.show();
        } else {
            card.hide();
        }

        this.refresh_();
    }));
};

/**
 * @param {Array} cards contains {string}
 * */
Hand.prototype.playCards = function(cards) {
    var playingCardViews = [];
    BOK.each(cards, function(card){
        BOK.each(this.cards_, function(cardView){
            if(cardView.value == card)
                playingCardViews.push(cardView);
        }, this);
    }, this);

    if(playingCardViews.length == cards.length) {
        this.removeCards_(playingCardViews);
    }

    //clear selected cards on plat action
    this.selectedCards_ = [];

    return playingCardViews;
};

/**
 * Check this hand belongs to the player that sits at the center of this table
 * @return {boolean}
 * */
Hand.prototype.isUserPlayer = function() {
    //user player sit in middle hence side is 0.
    return this.side_ == 0;
};

Hand.prototype.enable = function() {
    BOK.each(this.cards_, function(card){
        card.enable();
    });
};

Hand.prototype.disable = function() {
    BOK.each(this.cards_, function(card){
        card.disable();
    });
};


/**
 * @return {Array} contains {Cards}, removed cards
 * */
Hand.prototype.empty = function() {
    return this.removeCards_(this.cards_);
};

/**
 * @return {Array} contains {string}
 * */
Hand.prototype.getSelectedCards = function() {
    return this.selectedCards_;
};

Hand.prototype.resetSelectedCards = function() {
    BOK.each(this.cards_, function(card){
        if(card.isSelected()) {
            BOK.findAndRemove(this.selectedCards_, card.value);
            card.unselect(true);
        }
    }, this);
    this.selectedCards_.splice(0);
    this.dispatchEvent(new createjs.Event('selectedChange'));
};

/**
 * Refresh hand to display cards in proper order.
 * */
Hand.prototype.refresh_ = function() {
    BOK.each(this.cards_, function(card, index){
        card.x = this.x + this.stepX * index;
        card.y = this.y + this.stepY * index;
        card.parent.setChildIndex(card, card.parent.children.length-1);
    }, this);
};

/**
 * @param {Array} cards contains {Cards}
 * @return {Array} contains {Cards}, removed cards
 * */
Hand.prototype.removeCards_ = function(cards) {
    var removedCards = [];
    BOK.each(cards, function(card){
        if(BOK.findAndRemove(this.cards_, card) >= 0) {
            card.disable();
            removedCards.push(card);
            delete this.cardsIndex_[card.value];
        }
    }, this);

    this.refresh_();
    return removedCards;
};


/**
 * select cards
 * @param cards
 */
Hand.prototype.selectCards = function (cards) {
    this.resetSelectedCards();
    BOK.each(this.cards_, function (cardUI) {
        BOK.each(cards, function (selectedCard) {
            if (cardUI.value == selectedCard) {
                cardUI.select();
                CardHelper.insertCardToHand(cardUI.value, this.selectedCards_);
                return BOK.BREAK;
            }
        }, this);
    }, this);
    this.dispatchEvent(new createjs.Event('selectedChange'));
};


Hand.prototype.onCardSelected_ = function(e) {
    var card = e.target.value;

    CardHelper.insertCardToHand(card, this.selectedCards_);
    this.dispatchEvent(new createjs.Event('selectedChange'));
};
Hand.prototype.onCardUnselected_ = function(e) {
    var card = e.target;

    BOK.findAndRemove(this.selectedCards_, card.value);
    this.dispatchEvent(new createjs.Event('selectedChange'));
};

})();
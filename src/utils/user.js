class GameData {
  constructor() {
    this.currentNumber = 0.0;
    this.message = "";
  }
}

module.exports = class User {
  constructor() {
    this.lastDaily = new Date();
    this.balance = 10000;
    this.gameData = new GameData();
    this.bet = 0;
    this.prank = false;
  }
};
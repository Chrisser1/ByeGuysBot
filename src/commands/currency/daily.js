const { SlashCommandBuilder, MessageEmbed } = require("discord.js");
const Utils = require("./src/utils/utils.js");

module.exports = {
  data: new SlashCommandBuilder()
    .setName("daily")
    .setDescription("Gives a random amount of money"),
  async execute(interaction) {
    const user = global.Users.get(interaction.user.id);
    const now = Date.now();

    if (now - user.lastDaily >= 24 * 60 * 60 * 1000) {
      const spin = Utils.getRandomInt(1, 100);

      const embed = new MessageEmbed()
        .setTitle("Daily Rewards")
        .setFooter("Thanks for playing!", "https://e7.pngegg.com/pngimages/121/881/png-clipart-casino-roulette-casino-roulette-thumbnail.png")
        .setThumbnail("https://cdn.dribbble.com/users/975607/screenshots/6289727/wheel-of-fortune800_600.gif");

      if (user.prank) {
        if (spin <= 96) {
          user.balance += 100;
          embed.setColor("#0099ff")
            .addFields(
              { name: "Currency Won", value: "100", inline: true },
              { name: "New Balance", value: user.balance.toString(), inline: true },
            );
        }
        else if (spin <= 97) {
          user.balance += 200;
          embed.setColor("#33cc33")
            .addFields(
              { name: "Currency Won", value: "200", inline: true },
              { name: "New Balance", value: user.balance.toString(), inline: true },
            );
        }
        else if (spin <= 98) {
          user.balance += 500;
          embed.setColor("#ffff00")
            .addFields(
              { name: "Currency Won", value: "500", inline: true },
              { name: "New Balance", value: user.balance.toString(), inline: true },
            );
        }
        else if (spin <= 99) {
          user.balance += 1000;
          embed.setColor("#ff9900")
            .addFields(
              { name: "Currency Won", value: "1000", inline: true },
              { name: "New Balance", value: user.balance.toString(), inline: true },
            );
        }
        else {
          user.balance += 5000;
          embed.setColor("#ff0000")
            .addFields(
              { name: "Currency Won", value: "5000", inline: true },
              { name: "New Balance", value: user.balance.toString(), inline: true },
            );
        }
      }
      else if (spin <= 50) {
        user.balance += 100;
        embed.setColor("#0099ff")
          .addFields(
            { name: "Currency Won", value: "100", inline: true },
            { name: "New Balance", value: user.balance.toString(), inline: true },
          );
      }
      else if (spin <= 70) {
        user.balance += 200;
        embed.setColor("#33cc33")
          .addFields(
            { name: "Currency Won", value: "200", inline: true },
            { name: "New Balance", value: user.balance.toString(), inline: true },
          );
      }
      else if (spin <= 95) {
        user.balance += 500;
        embed.setColor("#ffff00")
          .addFields(
            { name: "Currency Won", value: "500", inline: true },
            { name: "New Balance", value: user.balance.toString(), inline: true },
          );
      }
      else if (spin <= 99) {
        user.balance += 1000;
        embed.setColor("#ff9900")
          .addFields(
            { name: "Currency Won", value: "1000", inline: true },
            { name: "New Balance", value: user.balance.toString(), inline: true },
          );
      }
      else {
        user.balance += 5000;
        embed.setColor("#ff0000")
          .addFields(
            { name: "Currency Won", value: "5000", inline: true },
            { name: "New Balance", value: user.balance.toString(), inline: true },
          );
      }

      user.lastDaily = Date.now();
      await interaction.reply({ embeds: [embed] });
    }
    else {
      const timeToWait = 24 * 60 * 60 * 1000 - (now - user.lastDaily);
      const hoursToWait = Math.floor(timeToWait / (60 * 60 * 1000));
      const minutesToWait = Math.floor((timeToWait % (60 * 60 * 1000)) / (60 * 1000));

      const embed = new MessageEmbed()
        .setTitle("Daily Rewards")
        .setColor("#FF0000")
        .setThumbnail("https://media.tenor.com/vTY0qobiAtsAAAAC/judge-judy-time.gif")
        .setDescription(`You have to wait 24 hours between each daily reward! You can claim your next reward in ${hoursToWait} hour(s) and ${minutesToWait} minute(s).`);

      await interaction.reply({ embeds: [embed] });
    }
  },
};
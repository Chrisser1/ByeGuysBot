const { SlashCommandBuilder, MessageEmbed } = require("discord.js");
const User = require("./src/utils/user.js");
const Utils = require("./src/utils/utils.js");

module.exports = {
  data: new SlashCommandBuilder()
    .setName("register")
    .setDescription("Register a new user"),
  async execute(interaction) {
    const currentID = interaction.user.id;

    if (global.Users.has(currentID)) {
      const embed = new MessageEmbed()
        // red color
        .setColor("#E74C3C")
        .setTitle("Already registered");
      await interaction.reply({ embeds: [embed] });
      return;
    }

    global.Users.set(currentID, new User.user());

    const embed = new MessageEmbed()
      // light blue color
      .setColor("#3498DB")
      .setTitle("Registered :hot_face: feet pic? :zany_face:");
    await interaction.reply({ embeds: [embed] });
    console.log(`${currentID}: ${Utils.getUser(currentID).userName}`);
  },
};

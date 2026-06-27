
export const MqttTopics = {
  ALL: "copa/partidas",
  TODAY: "copa/partidas/hoje",

  DETAILS: (id: number) => `copa/partidas/${id}`,
  CLOCK: (id: number) => `copa/partidas/${id}/tempo`,
  GOAL: (id: number) => `copa/partidas/${id}/gol`,
  STATUS: (id: number) => `copa/partidas/${id}/status`,
  COMMANDS: "copa/comandos",
} as const

import { MqttService } from "./app/mqtt/mqtt.service"
import { IMqttService } from "./app/mqtt/mqtt.service.interface"
import { WorldCupResultGateway } from "./infra/gateways/world-cup-results.gateway"
import { MqttTopics } from "./domain/constants/mqtt-topics.constants"
import { FetchLiveMatchesUseCase } from "./app/usecases/fetch-live-matches.usecase"
import { FetchLiveMatches } from "./app/usecases/fetch-live-matches"
import { MatchScheduler } from "./app/scheduler/match-scheduler"
import { IMatchScheduler } from "./app/scheduler/match-scheduler.interface"

async function main() {
  const gateway = new WorldCupResultGateway()
  const mqttService: IMqttService = new MqttService()

  await mqttService.connect()

  const fetchLiveMatches: FetchLiveMatchesUseCase = new FetchLiveMatches(gateway, mqttService)
  const scheduler: IMatchScheduler = new MatchScheduler(gateway, mqttService)

  await mqttService.subscribe(MqttTopics.COMMANDS)

  mqttService.onMessage(async (topic: string, message: string) => {
    if (topic !== MqttTopics.COMMANDS) return

    let command: { action: string; matchId?: number }

    try {
      command = JSON.parse(message)
    } catch {
      console.error("Comando inválido:", message)
      return
    }

    console.log("Comando recebido:", command)

    switch (command.action) {
      case "GET_ALL": {
        const matches = await gateway.getAllMatch()
        await mqttService.publish(MqttTopics.ALL, JSON.stringify(matches))
        break
      }

      case "GET_TODAY": {
        const matches = await gateway.getAllMatchToday()
        await mqttService.publish(MqttTopics.TODAY, JSON.stringify(matches))
        break
      }

      case "GET_LIVE": {
        const liveMatches = await fetchLiveMatches.execute()
        for (const match of liveMatches) {
          await scheduler.scheduleMatch(match.id)
        }
        break
      }

      case "WATCH": {
        if (!command.matchId) {
          console.error("WATCH requer matchId")
          return
        }
        const started = await scheduler.scheduleMatch(command.matchId)
        console.log(
          started
            ? `Schedule iniciado para partida ${command.matchId}`
            : `Partida ${command.matchId} já está sendo monitorada`
        )
        break
      }

      case "UNWATCH": {
        if (!command.matchId) {
          console.error("UNWATCH requer matchId")
          return
        }
        scheduler.stopSchedule(command.matchId)
        break
      }

      case "UNWATCH_ALL": {
        scheduler.stopAll()
        break
      }

      default:
        console.error("Ação desconhecida:", command.action)
    }
  })

  console.log(`Aguardando comandos em: ${MqttTopics.COMMANDS}`)

  const shutdown = async () => {
    console.log("Encerrando...")
    scheduler.stopAll()
    await mqttService.disconnect()
    process.exit(0)
  }

  process.on("SIGINT", shutdown)
  process.on("SIGTERM", shutdown)
}

main()

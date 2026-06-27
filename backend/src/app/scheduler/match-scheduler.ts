import { MqttTopics } from "../../domain/constants/mqtt-topics.constants"
import { IMatch, IMatchStatus } from "../../domain/entities/match.entity"
import { IWorldCupResultsGateway } from "../../infra/gateways/world-cup-results.gateway.interface"
import { IMqttService } from "../mqtt/mqtt.service.interface"
import { NotifyGoal } from "../usecases/notify-goal"
import { NotifyGoalUseCase } from "../usecases/notify-goal.usecase"
import { IMatchScheduler } from "./match-scheduler.interface"

export class MatchScheduler implements IMatchScheduler {
  private activeSchedulers: Map<number, NodeJS.Timeout> = new Map()
  private previousStates: Map<number, IMatch> = new Map()
  private readonly notifyGoal: NotifyGoalUseCase

  constructor(
    private readonly gateway: IWorldCupResultsGateway,
    private readonly mqtt: IMqttService,
  ) {
    this.notifyGoal = new NotifyGoal(mqtt)
  }

  async scheduleMatch(id: number): Promise<boolean> {
    if (this.activeSchedulers.has(id)) return false

    const runTick = async () => {
      try {
        const match = await this.gateway.getMatchFromId(id)

        if (
          match.status === IMatchStatus.FINISHED ||
          match.status === IMatchStatus.SCHEDULED
        ) {
          this.stopSchedule(id)
          return
        }

        const previous = this.previousStates.get(id)
        if (previous) {
          await this.notifyGoal.execute([previous], [match])
        }
        this.previousStates.set(id, match)

        await Promise.all([
          this.mqtt.publish(MqttTopics.DETAILS(id), JSON.stringify(match)),
          this.mqtt.publish(MqttTopics.CLOCK(id), JSON.stringify(match.gameTimeDisplay)),
          this.mqtt.publish(MqttTopics.STATUS(id), JSON.stringify(match.statusText)),
        ])
      } catch (error) {
        console.error(`Erro ao atualizar partida ${id}:`, error)
      }

      if (this.activeSchedulers.has(id)) {
        const timer = setTimeout(runTick, 1000);
        this.activeSchedulers.set(id, timer)
      }
    }

    const timer = setTimeout(runTick, 0)
    this.activeSchedulers.set(id, timer)
    return true
  }

  stopSchedule(id: number): void {
    const timer = this.activeSchedulers.get(id)
    if (timer) {
      clearTimeout(timer)
      this.activeSchedulers.delete(id)
      this.previousStates.delete(id)
      console.log(`Schedule da partida ${id} interrompido.`)
    }
  }

  stopAll(): void {
    this.activeSchedulers.forEach((_, id) => this.stopSchedule(id))
  }
}

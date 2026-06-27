import axios, { AxiosInstance } from "axios"
import "dotenv/config"
import { IWorldCupResultsGateway } from "./world-cup-results.gateway.interface"
import { IMatch, IMatchDetails, IMatchEvent, IMatchStatus } from "../../app/domain/entities/IMatch"
import { MatchNotFoundException } from "../shared/exceptions"

interface GamesResponse {
  games: any[]
}

interface GameResponse {
  game: any
}

export class WorldCupResultGateway implements IWorldCupResultsGateway {
  private client: AxiosInstance
  private readonly gamesEndpoint: string
  private readonly gameEndpoint: string

  constructor() {
    this.client = axios.create({
      baseURL: process.env.API_BASE_URL || "",
    })
    this.gamesEndpoint = process.env.API_GAMES_ENDPOINT || ""
    this.gameEndpoint = process.env.API_GAME_ENDPOINT || ""
  }

  async getAllMatch(): Promise<IMatch[]> {
    const response = await this.client.get<GamesResponse>(this.gamesEndpoint)
    return response.data.games.map((game) => this.toMatch(game))
  }

  async getAllMatchToday(): Promise<IMatch[]> {
    const matches = await this.getAllMatch()
    const today = this.formatDate(new Date())
    return matches.filter((match) => match.date === today)
  }

  async getMatchFromId(id: number): Promise<IMatchDetails> {

    const endpoint = this.gameEndpoint.replace(/gameId=\d+/, `gameId=${id}`)

    const response = await this.client.get<GameResponse>(endpoint).catch(
      () => {
        throw new MatchNotFoundException();
      }
    )
    return this.toMatchDetails(response.data.game)
  }

  private toMatch(data: any): IMatch {
    const date = new Date(data.startTime)
    return {
      id: data.id,
      shortHomeTeamName: data.homeCompetitor.symbolicName,
      shortAwayTeamName: data.awayCompetitor.symbolicName,
      homeScore: data.homeCompetitor.score ?? 0,
      homeColor: data.homeCompetitor.color,
      awayScore: data.awayCompetitor.score ?? 0,
      awayColor: this.resolveAwayColor(data),
      status: this.getGameStatus(data.statusGroup),
      minute: data.gameTime ?? 0,
      date: this.formatDate(date),
      time: this.formatTime(date),
    }
  }

  private toMatchDetails(data: any): IMatchDetails {
    return {
      ...this.toMatch(data),
      homeTeamId: data.homeCompetitor.id,
      awayTeamId: data.awayCompetitor.id,
      shortHomeTeamName: data.homeCompetitor.name,
      shortAwayTeamName: data.awayCompetitor.name,
      venue: data.venue?.name ?? "",
      statusText: data.statusText,
      gameTimeDisplay: data.gameTimeDisplay,
      events: this.toEvents(data.events ?? []),
    }
  }

  private toEvents(events: any[]): IMatchEvent[] {
    return events
      .filter((e) => e.eventType.id === 1)
      .map((e) => ({
        minute: e.gameTimeDisplay,
        competitorId: e.competitorId,
        type: e.eventType.name,
      }))
  }

  private resolveAwayColor(data: any): string {
    const homeColor = data.homeCompetitor.color
    const awayColor = data.awayCompetitor.awayColor
    return awayColor === homeColor ? data.awayCompetitor.color : awayColor
  }

  private getGameStatus(statusGroup: number): IMatchStatus {
    if (statusGroup === 3) return IMatchStatus.LIVE
    if (statusGroup === 4) return IMatchStatus.FINISHED
    return IMatchStatus.SCHEDULED
  }

  private formatDate(date: Date): string {
    return date.toLocaleDateString("pt-BR", {
      timeZone: "America/Sao_Paulo",
      day: "2-digit",
      month: "2-digit",
      year: "numeric",
    })
  }

  private formatTime(date: Date): string {
    return date.toLocaleTimeString("pt-BR", {
      timeZone: "America/Sao_Paulo",
      hour: "2-digit",
      minute: "2-digit",
    })
  }
}

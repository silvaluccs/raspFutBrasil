export enum IMatchStatus {
  SCHEDULED,
  LIVE,
  FINISHED,
}

export interface IMatch {
  id: number
  shortHomeTeamName: string
  shortAwayTeamName: string
  homeScore: number
  homeColor: string
  awayScore: number
  awayColor: string
  status: IMatchStatus
  minute: number
  date: string
  time: string
}

export interface IMatchEvent {
  minute: string
  competitorId: number
  type: string
}

export interface IMatchDetails extends IMatch {
  homeTeamId: number,
  awayTeamId: number,
  venue: string
  statusText: string
  gameTimeDisplay: string
  events: IMatchEvent[]
}

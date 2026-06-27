export interface IMatchScheduler {
  scheduleMatch(id: number): Promise<boolean>;
  stopSchedule(id: number): void;
  stopAll(): void;

}

export const IMatchScheduler = Symbol("IMatchScheduler");

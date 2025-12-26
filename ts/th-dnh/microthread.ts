type GeneratorType = Generator<void, void, void>;

export class MicrothreadManager {
  private nextTasks: Array<GeneratorType>;

  constructor() {
    this.nextTasks = [];
  }

  yield(): boolean {
    const tasks = this.nextTasks;
    this.nextTasks = [];
    tasks.forEach((task) => {
      try {
        const { done } = task.next();
        if (!done) {
          this.nextTasks.push(task);
        }
      } catch (e) {
        console.error(e);
      }
    });
    return this.nextTasks.length > 0;
  }

  startTask<Args extends unknown[]>(task: (...args: Args) => GeneratorType, ...args: Args): void {
    const gen = task(...args);
    const { done } = gen.next();
    if (!done) {
      this.nextTasks.push(gen);
    }
  }

  wrapTask<Args extends unknown[]>(task: (...args: Args) => GeneratorType): (...args: Args) => void {
    return (...args) => {
      this.startTask(task, ...args);
    };
  }

  countTasks(): number {
    return this.nextTasks.length;
  }
}

const globalManager = new MicrothreadManager();

export const yield_all = () => globalManager.yield();
export const startTask = <Args extends unknown[]>(task: (...args: Args) => GeneratorType, ...args: Args): void => {
  globalManager.startTask(task, ...args);
};
export const wrapTask = <Args extends unknown[]>(task: (...args: Args) => GeneratorType): ((...args: Args) => void) =>
  globalManager.wrapTask(task);

class Queue<T> {
  private stackPush: T[];
  private stackPop: T[];

  constructor() {
    this.stackPush = [];
    this.stackPop = [];
  }

  enqueue(value: T) {
    this.stackPush.push(value);
  }

  dequeue(): T | undefined {
    if (this.stackPop.length === 0) {
      while (this.stackPush.length > 0) {
        // eslint-disable-next-line @typescript-eslint/no-non-null-assertion
        this.stackPop.push(this.stackPush.pop()!);
      }
    }
    return this.stackPop.pop();
  }

  count(): number {
    return this.stackPush.length + this.stackPop.length;
  }
}

export class MicrothreadManager {
  private queue: Queue<() => void>;

  constructor() {
    this.queue = new Queue();
  }

  yield(): Promise<void> {
    return new Promise<void>((resolve) => {
      this.queue.enqueue(() => {
        resolve();
      });
      setTimeout(() => {
        const nextTask = this.queue.dequeue();
        if (nextTask != null) {
          nextTask();
        }
      }, 0);
      /*
      const nextTask = this.queue.dequeue();
      if (nextTask == null) {
        resolve();
        return;
      }
      this.queue.enqueue(() => {
        resolve();
      });
      nextTask();
       */
    });
  }

  startTask<Args extends unknown[]>(task: (...args: Args) => Promise<void>, ...args: Args): void {
    task(...args)
      .catch((err: unknown) => {
        console.error(err);
      })
      .finally(() => {
        const nextTask = this.queue.dequeue();
        if (nextTask != null) {
          nextTask();
        }
      });
  }

  wrapTask<Args extends unknown[]>(task: (...args: Args) => Promise<void>): (...args: Args) => void {
    return (...args) => {
      this.startTask(task, ...args);
    };
  }
}

const globalManager = new MicrothreadManager();

export const dnh_yield = (): Promise<void> => globalManager.yield();
export const startTask = <Args extends unknown[]>(task: (...args: Args) => Promise<void>, ...args: Args): void => {
  globalManager.startTask(task, ...args);
};
export const wrapTask = <Args extends unknown[]>(task: (...args: Args) => Promise<void>): ((...args: Args) => void) =>
  globalManager.wrapTask(task);

export interface WeatherDataPoint {
    temperature: number;
    humidity: number;
    pressure: number;
    timestamp: string;
    [key: string]: number | string;
}

type SmoothableKey = 'temperature' | 'humidity' | 'pressure';


export function exponentialSmoothing(
    data: { timestamp: string; temperature: number; humidity: number; pressure: number }[] | undefined,
    keys: SmoothableKey[],
    alpha: number
): WeatherDataPoint[] {
    if (alpha <= 0 || alpha > 1) {
        throw new Error("Alpha must be between 0 and 1.");
    }
    if (!data || data.length === 0) {
        return [];
    }

    return data.reduce((acc, currentPoint, index) => {
        if (index === 0) {
            acc.push({...currentPoint});
            return acc;
        }

        const prevSmoothedPoint = acc[index - 1];
        const newSmoothedPoint = {...currentPoint};

        for (const key of keys) {
            const prevSmoothedValue = prevSmoothedPoint[key] as number;
            const currentValue = currentPoint[key] as number;

            const smoothedValue = alpha * currentValue + (1 - alpha) * prevSmoothedValue;

            newSmoothedPoint[key] = parseFloat(smoothedValue.toFixed(2));
        }

        acc.push(newSmoothedPoint);
        return acc;
    }, [] as WeatherDataPoint[]);
}
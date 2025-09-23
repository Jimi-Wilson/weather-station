import {CartesianGrid, Line, LineChart, XAxis, YAxis} from "recharts"
import {
    Card,
    CardContent,
    CardDescription,
    CardHeader,
    CardTitle,
} from "@/components/ui/card"

import {
    type ChartConfig,
    ChartContainer,
    ChartTooltip,
    ChartTooltipContent,
} from "@/components/ui/chart"

import type {WeatherReading} from "@/services/types.ts";
import {Skeleton} from "@/components/ui/skeleton.tsx";

interface MetricChartProps {
    metric: 'temperature' | 'humidity' | 'pressure';
    data: WeatherReading[];
    isLoading?: boolean;
}

const metricChartConfigurations: ChartConfig = {
    temperature: {
        label: "Temperature (°C)",
        color: "hsl(var(--chart-1))",
    },

    humidity: {
        label: "Humidity (%)",
        color: "hsl(var(--chart-2))",
    },
    pressure: {
        label: "Pressure (hPa)",
        color: "hsl(var(--chart-3))",
    },
} satisfies ChartConfig;


const ChartSkeleton = () => (
    <Card>
        <CardHeader>
            <Skeleton className="h-5 w-32" />
            <Skeleton className="h-4 w-48" />
        </CardHeader>
        <CardContent>
            <div className="w-full aspect-video">
                <Skeleton className="h-full w-full" />
            </div>

        </CardContent>
    </Card>
);


const MetricChart = ({ metric, data, isLoading }: MetricChartProps) => {
    if (isLoading) {
        return <ChartSkeleton />;
    }

    const chartConfig = metricChartConfigurations[metric];

    const yAxisTickFormatter = (value: number) => {
        if (value > 100) {
            return Math.round(value).toString();
        }
        return value.toFixed(1);
    };

    return (
        <Card>
            <CardHeader>
                <CardTitle>{chartConfig.label}</CardTitle>
                <CardDescription>Last 24 hours</CardDescription>
            </CardHeader>
            <CardContent>
                <ChartContainer config={{ [metric]: chartConfig }} className="w-full aspect-video">
                    <LineChart data={data} margin={{ left: 12, right: 12 }}>
                        <CartesianGrid vertical={false} />
                        <XAxis
                            dataKey="timestamp"
                            tickLine={false}
                            axisLine={false}
                            tickMargin={8}

                        />
                        <YAxis
                            tickLine={false}
                            axisLine={false}
                            width={30}
                            tickFormatter={yAxisTickFormatter}
                            domain={['dataMin - 1', 'dataMax + 1']}
                        />
                        <ChartTooltip content={<ChartTooltipContent indicator="line" />} />
                        <Line
                            dataKey={metric}
                            type="natural"
                            strokeWidth={2}
                            dot={false}
                        />
                    </LineChart>
                </ChartContainer>
            </CardContent>
        </Card>
    );
};

export default MetricChart;

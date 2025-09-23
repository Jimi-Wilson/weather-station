import React from "react";
import {
    Card,
    CardContent,
    CardHeader,
    CardTitle,
} from "@/components/ui/card"
import {ArrowDown, ArrowUp, Minus} from "lucide-react";
import type {MetricStats} from "@/services/types.ts";

type MetricCardProps = {
    label: string;
    value: number | null;
    unit: string;
    icon: React.ReactNode;
    change: number | null;
    stats: MetricStats | null;
}

const cardStyles = {
    'temperature': {
        color: 'text-orange-500',
    },
    'humidity': {
        color: 'text-blue-500',
    },
    'pressure': {
        color: 'text-purple-500',
    }
}

type CardStyleKey = keyof typeof cardStyles;

const MetricCard = ({ label, value, unit, icon, change, stats}: MetricCardProps) => {
    const cardStyle = cardStyles[label.toLowerCase() as CardStyleKey];

    const getChangeIndicator = () => {
        if (change === null || change === undefined) {
            return null;
        }

        const roundedChange = parseFloat(change.toFixed(1));

        let colorClass: string;
        let ChangeIcon: React.ReactNode;

        if (roundedChange > 0) {
            colorClass = 'text-green-500';
            ChangeIcon = <ArrowUp size={16} />;
        } else if (roundedChange < 0) {
            colorClass = 'text-red-500';
            ChangeIcon = <ArrowDown size={16} />;
        } else {
            colorClass = 'text-slate-500';
            ChangeIcon = <Minus size={16} />;
        }


        return (
            <div className={`flex items-center gap-1 font-semibold ${colorClass}`}>
                {ChangeIcon}
                <span>{Math.abs(roundedChange)}</span>
            </div>
        );
    };

    return (
        <Card>
            <CardHeader>
                <CardTitle>
                    <div className="flex items-center gap-2">
                        <div className={cardStyle.color}>
                            {icon}
                        </div>
                        <p>
                            {label}
                        </p>
                    </div>
                </CardTitle>
            </CardHeader>

            <CardContent>
                <div className="flex justify-between items-baseline mb-4">
                    <p className="text-3xl font-bold">
                        {value === null ? "N/A" : `${value.toFixed(1)} ${unit}`}
                    </p>
                    {getChangeIndicator()}
                </div>

                {stats && (
                    <div className="text-sm text-slate-500 dark:text-slate-400 space-y-1 pt-4 border-t">
                        <p className="font-semibold text-slate-600 dark:text-slate-300 mb-2">Today's Stats</p>
                        <div className="flex justify-between items-center">
                            <span className="flex items-center gap-1"><ArrowUp size={14}/> Max</span>
                            <span>{stats.maximum?.toFixed(1) ?? 'N/A'} {unit}</span>
                        </div>
                        <div className="flex justify-between items-center">
                            <span className="flex items-center gap-1"><Minus size={14}/> Avg</span>
                            <span>{stats.average?.toFixed(1) ?? 'N/A'} {unit}</span>
                        </div>
                        <div className="flex justify-between items-center">
                            <span className="flex items-center gap-1"><ArrowDown size={14}/> Min</span>
                            <span>{stats.minimum?.toFixed(1) ?? 'N/A'} {unit}</span>
                        </div>
                    </div>
                )}
            </CardContent>
        </Card>

    );


}


export default MetricCard;